from defs import *
from ftmbdefs import *
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


def shared_state_declare(chain_pos, thrds, mb, sharing_level, batch, perf_met):

    if chain_pos % 2 == 1:  # No need for shared state for output loggers
        return '// No shared state'

    dst_index = chain_pos + 1

    data_dev = None
    if perf_met == THROUGHPUT_METRIC:
        data_dev = '40'
    elif perf_met == LATENCY_METRIC:
        data_dev = '10'

    if mb == COUNTER:
        return SHARED_STATE_COUNTER_FORMAT_STR.format(**{
            LOCKS: COUNTER_LOCKS,
            BATCH: batch,
            QUEUES: thrds,
            SHARING_LEVEL: sharing_level,
            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(dst_index, data_dev),
            DATA_DST_IP: dev_ip(dst_index, data_dev),
        })

    elif mb == NAT:
        return SHARED_STATE_NAT_FORMAT_STR.format(**{
            LOCKS: NAT_LOCKS,
            BATCH: batch,
            QUEUES: thrds,
            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(dst_index, data_dev),
            DATA_DST_IP: dev_ip(dst_index, data_dev),
        })

    elif mb == LB:
        return SHARED_STATE_FORMAT_STR.format(**{
            LOCKS: LB_LOCKS,
            BATCH: batch,
            QUEUES: thrds,
            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(dst_index, data_dev),
            DATA_DST_IP: dev_ip(dst_index, data_dev),
        })

    return None


def dev_name_list(from_or_to, device, thrds):
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
            })
        )
    return result


def fd_data_names_list(thrds):
    """
    list of data FromDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('from', DATA_DEVICE_ID, thrds)


def fd_state_names_list(thrds):
    """
    list of state FromDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('from', STATE_DEVICE_ID, thrds)


def fd_names_list(chain_pos, thrds):
    """
    list of FromDevice element names based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: the list of device names
    """
    name_list = fd_data_names_list(thrds)

    return name_list


def td_data_names_list(thrds):
    """
    list of data ToDevice element names
    :param thrds: a number denoting the number of threads
    :return: list of names
    """
    return dev_name_list('to', DATA_DEVICE_ID, thrds)


def td_names_list(thrds):
    """
    list of ToDevice element names based on the position of middlebox in the chain
    and the number of threads
    :param thrds: a number denoting the number of threads
    :return: the list of device names
    """
    name_list = td_data_names_list(thrds)

    return name_list


def dev_declares(from_or_to, chain_pos, thrds, perf_met):
    """
    format device declares based on the position of middlebox in the chain
    and the number of threads
    :param from_or_to: FromDevice or ToDevice
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of device declares
    """
    def inner_dev_list(dev_format_str, device, thrds):
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
                })
            )
        return result

    # If the middlebox at the beginning of the chain, then
    # it receives traffic from two NICs

    if from_or_to == 'from':
        dev_format_str = FROM_DEVICE_FORMAT_STR
    else:
        dev_format_str = TO_DEVICE_FORMAT_STR

    data_dev_id = None
    if perf_met == LATENCY_METRIC:
        data_dev_id = LATENCY_DATA_DEVICE_ID
    elif perf_met == THROUGHPUT_METRIC:
        data_dev_id = THROUGHPUT_DATA_DEVICE_ID

    devs = inner_dev_list(dev_format_str, data_dev_id, thrds)

    return '\n'.join(devs)


def from_dev_declares(chain_pos, thrds, perf_met):
    """
    format from device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of from device declares
    """
    return dev_declares('from', chain_pos, thrds, perf_met)


def to_dev_declares(chain_pos, thrds, perf_met):
    """
    format to device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of the to device declares
    """
    return dev_declares('to', chain_pos, thrds, perf_met)


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
    the list of FTMB block names
    :param thrds: a number denoting the number of threads
    :return: names list
    """
    return [FTMB_BLOCK_NAME_FORMAT_STR.format(i) for i in range(thrds)]


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
    return "1.{}.{}.0".format(
        chain_pos + 1,
        thrd)


def gen_mb_params_str(thrds, chain_pos, thrd, mb):
    if chain_pos % 2 == 1:
        return ''

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

    elif mb == LB:
        return ''

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
    if chain_pos % 2 == 0:
        format_str = FTMB_MASTER_BLOCK_FORMAT_STR
    else:
        format_str = FTMB_SLAVE_BLOCK_FORMAT_STR

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

    ft_block_names = ft_block_names_list(thrds)

    ll = []
    for i in range(thrds):
        ll.append(LINK_FORMAT_STR.format(**{
            QUEUE: i,
            'FROM_DATA_DEVICE_NAME': fd_data_names[i],
            'FTMB_BLOCK_NAME': ft_block_names[i],
            'TO_DATA_DEVICE_NAME': td_data_names[i],
        }))

    return '\n'.join(ll)


def middlebox_declare(mb):
    result = None
    if mb == NAT:
        result = NAT_MB

    elif mb == COUNTER:
        result = COUNTER_MB

    elif mb == LB:
        result = BEAMER_MUX_MB

    return result


def ftmb_block_def(ch_len, thrds, chain_pos, mb, perf_met):
    """
    format a block declare
    :param chain_pos: a number denoting the position of middlebox in the chain
    :return: formatted string of ft block
    """
    data_dev = None
    if perf_met == THROUGHPUT_METRIC:
        data_dev = '40'
    elif perf_met == LATENCY_METRIC:
        data_dev = '10'

    if chain_pos == -1 or chain_pos == (ch_len - 1):
        dst_index = -FIRST_AQUA_MACHINE_IN_CHAIN
        src_ip_filter_index = ch_len - 2

    else:
        dst_index = chain_pos + 1
        src_ip_filter_index = chain_pos - 1

    if chain_pos % 2 == 1:
        return FTMB_SLAVE_BLOCK.format(**{
            SRC_IP_FILTER: src_ip_filter(src_ip_filter_index),
            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(dst_index, data_dev),
            DATA_DST_IP: dev_ip(dst_index, data_dev),
            DATA_SRC_NAME: dev_name(chain_pos),
            DATA_DST_NAME: dev_name(dst_index),
        })

    if mb == COUNTER:
        mb_params = COUNTER_MB_PARAMS

    elif mb == NAT:
        mb_params = NAT_MB_PARAMS

    elif mb == LB:
        mb_params = BEAMER_MUX_MB_PARAMS

    mb_params_str = ''
    if mb_params is not None and len(mb_params) > 0:
        mb_params_str = ', ' + ', '.join(mb_params)

    return FTMB_MASTER_BLOCK.format(**{
            SRC_IP_FILTER: src_ip_filter(src_ip_filter_index),

            MB: middlebox_declare(mb),
            MB_PARAM: mb_params_str,

            DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
            DATA_DST_MAC: dev_mac(dst_index, data_dev),
            DATA_DST_IP: dev_ip(dst_index, data_dev),
            DATA_SRC_NAME: dev_name(chain_pos),
            DATA_DST_NAME: dev_name(dst_index),
        })


def ftmb_click(ch_len, chain_pos, thrds, mb, sharing_level, batch, perf_met):
    """
    Click code for FTMB
    :return: Click code in string
    """
    string_map = {
        SHARED_STATE_DECLARE: shared_state_declare(chain_pos,
                                                   thrds,
                                                   mb,
                                                   sharing_level,
                                                   batch,
                                                   perf_met),

        FTMB_BLOCK_DEF: ftmb_block_def(ch_len,
                                       thrds,
                                       chain_pos,
                                       mb,
                                       perf_met),

        FROM_DEVICE_DECLARES: from_dev_declares(chain_pos,
                                                thrds,
                                                perf_met),

        TO_DEVICE_DECLARES: to_dev_declares(chain_pos,
                                            thrds,
                                            perf_met),

        THREAD_SCHEDULES: thread_sched_declare(fd_names_list(chain_pos,
                                                             thrds)),

        FTMB_BLOCKS_DECLARES: ft_blocks_declares(chain_pos,
                                                 ch_len,
                                                 thrds,
                                                 mb),

        LINKS: links(ch_len,
                     chain_pos,
                     thrds),
    }

    return FTMB.format(**string_map)


def generate(ch_len, thrds, mb, sharing_level, batch, perf_met):
    clicks = []
    ch_len *= 2
    if len(mb) == 1:
        mb *= ch_len
    elif len(mb) != ch_len:
        raise ValueError("The number of middleboxes must be either 1 or equal to chain length!")

    for chain_pos in range(ch_len):
        clicks.append(ftmb_click(ch_len, chain_pos, thrds, mb[chain_pos // 2], sharing_level, batch, perf_met))

    return clicks


def store(output_path, clicks):
    for i in range(len(clicks)):
        file_name = os.path.join(output_path, FTMB_OUTPUT_NAME_FORMAT_STR.format(i + 1))
        with open(file_name, 'w') as handle:
            handle.write(clicks[i])
