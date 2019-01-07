from defs import *
from ftcdefs import *
import os
import socket
import struct


def divide_ip_space(thrds, chain_pos):
    def ip2long(ip):
        """
        Convert an IP string to long
        :param ip: ip in string
        :return: a long value of ip
        """
        packed_ip = socket.inet_aton(ip)
        return struct.unpack("!L", packed_ip)[0]

    def long2ip(ip_l):
        """
        Convert an IP long to IP string
        :param ip_l: IP in long
        :return: IP string
        """
        return socket.inet_ntoa(struct.pack('!L', ip_l))

    if chain_pos == 0:
        s = ip2long(IN_TR_SRC_IP_MIN)
        t = ip2long(IN_TR_SRC_IP_MAX)
    else:
        s = ip2long(src_ip_filter(chain_pos, 0))
        t = ip2long(src_ip_filter(chain_pos, thrds - 1))

    step = (t - s) / thrds

    ll = []
    for i in range(0, thrds):
        ll.append(long2ip(s + i * step))
        ll.append(long2ip(s + (i + 1) * step - 1))

    ll[-1] = long2ip(t)

    return ll


def shared_state_declare(ch_len, chain_pos, f, mb, sharing_level):
    if chain_pos == -1:
        chain_pos = ch_len - 1

    if mb == COUNTER:
        return SHARED_STATE_COUNTER_FORMAT_STR.format(**{
            CHAIN: ch_len,
            ID: chain_pos,
            F: f,
            SHARING_LEVEL: sharing_level,
            LOCKS: 8,
        })

    elif mb == NAT:
        return SHARED_STATE_NAT_FORMAT_STR.format(**{
            LOCKS: 32768,
            CHAIN: ch_len,
            ID: chain_pos,
            F: f,
        })

    return SHARED_STATE_FORMAT_STR.format(**{
        CHAIN: ch_len,
        ID: chain_pos,
        F: f,
        LOCKS: 32768,
    })


def dev_name_list(from_or_to, device, thrds, channel):
    """
    list of device element names
    :param from_or_to: FromDevice or ToDevice
    :param device: the exact device id
    :param thrds: a number denoting the number of threads
    :param channel: state or data channel
    :return: list of names
    """
    result = []
    format_str = ''
    if from_or_to == 'from':
        format_str = FROM_DEVICE_NAME_FORMAT_STR
    elif from_or_to == 'to':
        format_str = TO_DEVICE_NAME_FORMAT_STR

    for queue in range(thrds):
        result.append(
            format_str.format(**{
                DEVICE: device,
                QUEUE: queue,
                CHANNEL: channel
            })
        )
    return result


def fd_data_names_list(thrds):
    """
    list of data FromDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('from', DATA_DEVICE_ID, thrds, DATA)


def fd_state_names_list(thrds):
    """
    list of state FromDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('from', STATE_DEVICE_ID, thrds, STATE)


def fd_names_list(ch_len, chain_pos, thrds):
    """
    list of FromDevice element names based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: the list of device names
    """
    name_list = fd_data_names_list(thrds)

    if chain_pos == 0 and ch_len > 1:
        name_list.extend(fd_state_names_list(thrds))

    return name_list


def td_data_names_list(thrds):
    """
    list of data ToDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('to', DATA_DEVICE_ID, thrds, DATA)


def td_state_names_list(thrds):
    """
    list of state FromDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('to', STATE_DEVICE_ID, thrds, STATE)


def td_names_list(chain_pos, thrds):
    """
    list of ToDevice element names based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: the list of device names
    """
    name_list = td_data_names_list(thrds)

    if chain_pos == -1:
        name_list.extend(td_state_names_list(thrds))

    return name_list


def dev_declares(ch_len, from_or_to, chain_pos, thrds):
    """
    format device declares based on the position of middlebox in the chain
    and the number of threads
    :param from_or_to: FromDevice or ToDevice
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of device declares
    """
    def inner_dev_list(dev_format_str, device, thrds, channel):
        """
        Helper function for multiple threads
        :param dev_format_str: device format string
        :param device: the exact device id
        :param thrds: a number denoting the number of threads
        :return: list of device declares
        """
        result = []
        for queue in range(thrds):
            result.append(
                dev_format_str.format(**{
                    DEVICE: device,
                    QUEUE: queue,
                    CHANNEL: channel
                })
            )
        return result

    # If the middlebox at the beginning of the chain, then
    # it receives traffic from two NICs

    if from_or_to == 'from':
        dev_format_str = FROM_DEVICE_FORMAT_STR
    else:
        dev_format_str = TO_DEVICE_FORMAT_STR

    devs = inner_dev_list(dev_format_str, DATA_DEVICE_ID, thrds, DATA)

    if (ch_len > 1) and (
            (chain_pos == 0 and from_or_to == 'from') or
            (chain_pos == -1 and from_or_to == 'to')):
        devs.extend(inner_dev_list(dev_format_str, STATE_DEVICE_ID, thrds, STATE))

    return '\n'.join(devs)


def from_dev_declares(ch_len, chain_pos, thrds):
    """
    format from device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain, use -1 for the last middlebox
    :param thrds: a number denoting the number of threads
    :return: formatted string of from device declares
    """
    return dev_declares(ch_len, 'from', chain_pos, thrds)


def to_dev_declares(ch_len, chain_pos, thrds):
    """
    format to device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of the to device declares
    """
    return dev_declares(ch_len, 'to', chain_pos, thrds)


def thread_sched_declare(from_devs_list):
    """
    Format thread schedule declares from the list of FromDeviceDeclares
    :param from_devs_list:
    :return: formatted string of the thread schedule
    """
    result = []
    for i in range(len(from_devs_list)):
        result.append("{} {}".format(from_devs_list[i], i % CPUS))

    return THREAD_SCHED_FORMAT_STR.format(', '.join(result))


def ft_block_names_list(thrds):
    """
    the list of FTC block names
    :param thrds: a number denoting the number of threads
    :return: names list
    """
    return [FTC_BLOCK_NAME_FORMAT_STR.format(i) for i in range(thrds)]


def dev_name(chain_pos):
    """
    The name of the server based on the given position of middlebox in the chain
    :param chain_pos: a number denoting the position of middlebox in the chain
    :return: server name
    """
    return AQUA_MACHINES[chain_pos + FIRST_AQUA_MACHINE_IN_CHAIN]


def dev_ip(chain_pos, _40_or_10):
    """
    IP address of the device based on the given position of middlebox in the chain
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param _40_or_10: which NIC, 10 or 40
    :return: device ip
    """
    result = ''
    if _40_or_10 == '40':
        result = NIC_40G_IP_LIST[chain_pos + FIRST_AQUA_MACHINE_IN_CHAIN]
    elif _40_or_10 == '10':
        result = NIC_10G_IP_LIST[chain_pos + FIRST_AQUA_MACHINE_IN_CHAIN]

    return result


def dev_mac(chain_pos, _40_or_10):
    """
    IP address of the device based on the given position of middlebox in the chain
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param _40_or_10: which NIC, 10 or 40
    :return: device ip
    """
    result = ''
    if _40_or_10 == '40':
        result = NIC_40G_MAC_LIST[chain_pos + FIRST_AQUA_MACHINE_IN_CHAIN]
    elif _40_or_10 == '10':
        result = NIC_10G_MAC_LIST[chain_pos + FIRST_AQUA_MACHINE_IN_CHAIN]

    return result


def src_ip_filter(chain_pos, thrd=0):
    if chain_pos == -1:
        return "2.0.{}.0".format(
            thrd)
    return "1.{}.{}.0".format(
        chain_pos + 1,
        thrd)


def gen_mb_params_str(thrds, chain_pos, thrd, mb):
    if mb == COUNTER:
        return thrd
    elif mb == NAT:
        ips = divide_ip_space(thrds, chain_pos)
        ip_min = ips[thrd * 2]
        ip_max = ips[thrd * 2 + 1]
        return ', '.join([
            ip_min,
            ip_max
        ])

    return thrd


def ft_blocks_declares(chain_pos, ch_len, thrds, mb):
    """
    format ft block declares based on the position of middlebox in the chain, number of threads, and
    mb parameters
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param ch_len: the chain's length
    :param thrds: a number denoting the number of threads
    :return: formatted string of the ft_blocks
    """
    declares = []
    format_str = FTC_BLOCK_FORMAT_STR

    if chain_pos == -1 and ch_len == 1:
        format_str = FTC_BLOCK_SINGLE2_FORMAT_STR

    elif chain_pos == -1:
        format_str = FTC_BLOCK_WITH_BUFFER_FORMAT_STR

    for thrd in range(thrds):
        mb_p = gen_mb_params_str(thrds, chain_pos, thrd, mb)
        if mb_p != '':
            mb_p = ', ' + str(mb_p)

        params = {
            QUEUE: thrd,
            MB_PARAMS: mb_p,
        }
        if chain_pos == -1:
            params[STATE_SRC_IP] = src_ip_filter(chain_pos, thrd)
            params[DATA_SRC_IP] = src_ip_filter(ch_len - 1, thrd)
        else:
            params[DATA_SRC_IP] = src_ip_filter(chain_pos, thrd)

        declares.append(
            format_str.format(**params)
        )

    return '\n'.join(declares)


def links(ch_len, chain_pos, thrds):
    """
    format link strings, each link is 'FromDevice... -> FTBlock -> ...ToDevice'
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of links
    """
    fd_data_names = fd_data_names_list(thrds)
    td_data_names = td_data_names_list(thrds)

    fd_state_names = fd_state_names_list(thrds)
    td_state_names = td_state_names_list(thrds)

    ft_block_names = ft_block_names_list(thrds)

    format_str = LINK_FORMAT_STR
    if ch_len > 1 and chain_pos == 0:
        format_str = LINK_WITH_FORWARDER_FORMAT_STR
    elif ch_len > 1 and chain_pos == -1:
        format_str = LINK_WITH_BUFFER_FORMAT_STR

    ll = []
    for i in range(thrds):
        params = {
            QUEUE: i,
            'FROM_DATA_DEVICE_NAME': fd_data_names[i],
            'FTC_BLOCK_NAME': ft_block_names[i],
            'TO_DATA_DEVICE_NAME': td_data_names[i],
        }
        if ch_len > 1:
            if chain_pos == 0:
                params['FROM_STATE_DEVICE_NAME'] = fd_state_names[i]
            elif chain_pos == -1:
                params['TO_STATE_DEVICE_NAME'] = td_state_names[i]

        ll.append(format_str.format(**params))

    return '\n'.join(ll)


def middlebox_declare(mb):
    result = None
    if mb == NAT:
        result = NAT_MB

    elif mb == COUNTER:
        result = COUNTER_MB

    return result


def ft_block_def(ch_len, thrds, chain_pos, mb, batch):
    """
    format a block declare
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param mb_params: the list of parameter lists for MB's threads
    :return: formatted string of ft block
    """
    data_dev = '40'
    state_dev = '10'

    mb_params = []
    if mb == COUNTER:
        mb_params = COUNTER_MB_PARAMS
    elif mb == NAT:
        mb_params = NAT_MB_PARAMS

    mb_params_str = ''
    if mb_params is not None and len(mb_params) > 0:
        mb_params_str = ', ' + ', '.join(mb_params)

    if ch_len == 1 and chain_pos == 0:
        result = FTC_BLOCK_SINGLE1.format(**{
            MB: middlebox_declare(mb),
            MB_PARAM: mb_params_str,
            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(chain_pos + 1, data_dev),
            DATA_DST_IP: dev_ip(chain_pos + 1, data_dev),
            DATA_SRC_NAME: dev_name(chain_pos),
            DATA_DST_NAME: dev_name(chain_pos + 1),
        })

    elif ch_len == 1 and chain_pos == -1:
        result = FTC_BLOCK_SINGLE2.format(**{
            DATA_SRC_MAC: dev_mac(ch_len - 1, data_dev),
            DATA_DST_MAC: dev_mac(-FIRST_AQUA_MACHINE_IN_CHAIN, data_dev),
            DATA_DST_IP: dev_ip(-FIRST_AQUA_MACHINE_IN_CHAIN, data_dev),
            DATA_SRC_NAME: dev_name(ch_len - 1),
            DATA_DST_NAME: dev_name(-FIRST_AQUA_MACHINE_IN_CHAIN),
            SRC_IP_FILTER: src_ip_filter(0),
        })

    elif chain_pos == 0:
        result = FTC_BLOCK_WITH_FORWARDER.format(**{
            CHAIN: ch_len,

            MB: middlebox_declare(mb),
            MB_PARAM: mb_params_str,

            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(chain_pos + 1, data_dev),
            DATA_DST_IP: dev_ip(chain_pos + 1, data_dev),
            DATA_SRC_NAME: dev_name(chain_pos),
            DATA_DST_NAME: dev_name(chain_pos + 1),
        })

    elif chain_pos == -1 or chain_pos == (ch_len - 1):
        result = FTC_BLOCK_WITH_BUFFER.format(**{
            BATCH: batch,
            CHAIN: ch_len,

            MB: middlebox_declare(mb),
            MB_PARAM: mb_params_str,

            SRC_IP_FILTER: src_ip_filter(ch_len - 2),

            DATA_SRC_MAC: dev_mac(ch_len - 1, data_dev),
            DATA_DST_MAC: dev_mac(-FIRST_AQUA_MACHINE_IN_CHAIN, data_dev),
            DATA_SRC_IP: dev_ip(ch_len - 1, data_dev),
            DATA_DST_IP: dev_ip(-FIRST_AQUA_MACHINE_IN_CHAIN, data_dev),

            STATE_SRC_MAC: dev_mac(ch_len - 1, state_dev),
            STATE_DST_MAC: dev_mac(0, state_dev),
            STATE_SRC_IP: dev_ip(ch_len - 1, state_dev),
            STATE_DST_IP: dev_ip(0, state_dev),

            DATA_SRC_NAME: dev_name(ch_len - 1),
            DATA_DST_NAME: dev_name(-FIRST_AQUA_MACHINE_IN_CHAIN),

            STATE_SRC_NAME: dev_name(ch_len - 1),
            STATE_DST_NAME: dev_name(0),
        })

    else:
        result = FTC_BLOCK.format(**{
            SRC_IP_FILTER: src_ip_filter(chain_pos - 1),

            MB: middlebox_declare(mb),
            MB_PARAM: mb_params_str,

            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(chain_pos + 1, data_dev),
            DATA_DST_IP: dev_ip(chain_pos + 1, data_dev),
            DATA_SRC_NAME: dev_name(chain_pos),
            DATA_DST_NAME: dev_name(chain_pos + 1),
        })

    return result


def ftc_click(ch_len, chain_pos, thrds, mb, sharing_level, f, batch):
    """
    Click code for FTC
    :return: Click code in string
    """
    string_map = {
        SHARED_STATE_DECLARE: shared_state_declare(ch_len,
                                                   chain_pos,
                                                   f,
                                                   mb,
                                                   sharing_level),

        FTC_BLOCK_DEF: ft_block_def(ch_len,
                                    thrds,
                                    chain_pos,
                                    mb,
                                    batch),

        FROM_DEVICE_DECLARES: from_dev_declares(ch_len,
                                                chain_pos,
                                                thrds),

        TO_DEVICE_DECLARES: to_dev_declares(ch_len,
                                            chain_pos,
                                            thrds),

        THREAD_SCHEDULES: thread_sched_declare(fd_names_list(ch_len,
                                                             chain_pos,
                                                             thrds)),

        FTC_BLOCKS_DECLARES: ft_blocks_declares(chain_pos,
                                                ch_len,
                                                thrds,
                                                mb),

        LINKS: links(ch_len,
                     chain_pos,
                     thrds),
    }

    return FTC.format(**string_map)


def generate(ch_len, thrds, mb, sharing_level, f, batch):
    clicks = []
    if len(mb) == 1:
        mb *= ch_len
    elif len(mb) != ch_len:
        raise ValueError("The number of middleboxes list must be either 1 or equal to chain length!")

    for_loop = ch_len - 1
    if ch_len == 1:
        for_loop = 1
    for chain_pos in range(for_loop):
        clicks.append(ftc_click(ch_len, chain_pos, thrds, mb[chain_pos], sharing_level, f, batch))

    clicks.append(ftc_click(ch_len, -1, thrds, mb[-1], sharing_level, f, batch))

    return clicks


def store(output_path, clicks):
    for i in range(len(clicks)):
        file_name = os.path.join(output_path, FTC_OUTPUT_NAME_FORMAT_STR.format(i + 1))
        with open(file_name, 'w') as handle:
            handle.write(clicks[i])
