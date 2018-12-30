#ifndef CLICK_CLICKITYCLACK_STATETRACK
#define CLICK_CLICKITYCLACK_STATETRACK

#include <click/glue.hh>
#include <click/config.h>
#include <click/list.hh>
#include <click/hashtable.hh>

CLICK_DECLS

namespace ClickityClack
{

template <typename K> struct State
{
	typedef K Key;
	
	List_member<State> queueMember;
	click_jiffies_t expiryTime;
	
	Key key;
	
	State() {}
	
	State(K key)
		: key(key) {}
};

template <typename T> class StateTrack
{
public:
	typedef typename T::Key Key;
	typedef typename List<State<Key>, &State<Key>::queueMember>::const_iterator const_iterator;
	
private:
	click_jiffies_t expiryPeriod;
	List<State<Key>, &State<Key>::queueMember> expiryQueue;
	
	HashTable<Key, T *> states;
	size_t maxEntries;
	
	SizedHashAllocator<sizeof(T)> allocator;
	
public:
	StateTrack(click_jiffies_t expiryPeriod = 4 * 60 * CLICK_HZ, size_t maxEntries = -1)
		: expiryPeriod(expiryPeriod), maxEntries(maxEntries)
	{
		if (maxEntries != -1)
			preallocate(maxEntries + 1);
	}
	
	T *allocate()
	{
		return reinterpret_cast<T *>(allocator.allocate());
	}
	
	void deallocate(T *victim)
	{
		allocator.deallocate(victim);
	}
	
	void preallocate(size_t count)
	{
		void **entries = new void *[count];
	
		for (int i = 0; i < count; i++)
			entries[i] = allocator.allocate();
	
		for (int i = 0; i < count; i++)
			allocator.deallocate(entries[i]);
	
		delete entries;
	}
	
	T *get(Key key)
	{
		return states.get(key);
	}
	
	bool put(T *state, click_jiffies_t now = click_jiffies())
	{
		if (states.size() == maxEntries)
			return false;
		
		states[state->key] = state;
		
		state->expiryTime = now + expiryPeriod;
		expiryQueue.push_back(state);
		
		return true;
	}
	
	bool contains(Key key) const
	{
		return states.find(key) != states.end();
	}
	
	void refresh(T *state, click_jiffies_t now = click_jiffies())
	{
		state->expiryTime = now + expiryPeriod;
		expiryQueue.erase(state);
		expiryQueue.push_back(state);
	}
	
	inline void remove(T *state)
	{
		expiryQueue.erase(state);
		states.erase(state->key);
		state->~T();
		deallocate(state);
		//click_chatter("expired");
	}
	
	void checkExpiration(click_jiffies_t now = click_jiffies())
	{
		while (!expiryQueue.empty())
		{
			State<Key> *base = expiryQueue.front();
			T *entry = (T *)base;
	
			if (now < entry->expiryTime)
				break;
	
			remove(entry);
		}
	}

	void setExpiryPeriod(click_jiffies_t period)
	{
		expiryPeriod = period;
	}

	click_jiffies_t getExpiryPeriod()
	{
		return expiryPeriod;
	}
	
	void clear()
	{
		while (!expiryQueue.empty())
		{
			State<Key> *base = expiryQueue.front();
			T *entry = (T *)base;
			
			remove(entry);
		}
	}
	
	const_iterator begin() const
	{
		return expiryQueue.begin();
	}
	
	const_iterator end() const
	{
		return expiryQueue.end();
	}
	
	bool empty() const
	{
		return expiryQueue.empty();
	}
	
	size_t size() const
	{
		return expiryQueue.size();
	}
	
	T *getBestEffort(Key key, click_jiffies_t now = click_jiffies())
	{
		T *ret = states.get(key);
		
		if (!ret)
			return NULL;
		
		if (now > ret->expiryTime)
		{
			remove(ret);
			return NULL;
		}
		
		return ret;
	}
	
	void putBestEffort(T *state, click_jiffies_t now = click_jiffies())
	{
		if (states.size() == maxEntries)
		{
			State<Key> *base = expiryQueue.front();
			T *entry = (T *)base;
	
			remove(entry);
		}
		
		states[state->key] = state;
		
		state->expiryTime = now + expiryPeriod;
		expiryQueue.push_back(state);
	}
};

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_STATETRACK */