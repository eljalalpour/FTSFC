from defs import *
from nfdefs import *
import os


def shared_state_declare():
    return SHARED_STATE_FORMAT_STR


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

    if chain_pos == 0:
        name_list.extend(fd_state_names_list(thrds))

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


def dev_declares(from_or_to, chain_pos, thrds):
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

    devs = inner_dev_list(dev_format_str, DATA_DEVICE_ID, thrds)

    return '\n'.join(devs)


def from_dev_declares(chain_pos, thrds):
    """
    format from device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of from device declares
    """
    return dev_declares('from', chain_pos, thrds)


def to_dev_declares(chain_pos, thrds):
    """
    format to device declares based on the position of middlebox in the chain
    and the number of threads
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param thrds: a number denoting the number of threads
    :return: formatted string of the to device declares
    """
    return dev_declares('to', chain_pos, thrds)


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


def nf_block_names_list(thrds):
    """
    the list of FTC block names
    :param thrds: a number denoting the number of threads
    :return: names list
    """
    return [NF_BLOCK_NAME_FORMAT_STR.format(i) for i in range(thrds)]


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
    return "1.{}.{}.{}".format(
        chain_pos + 1,
        thrd + 1,
        thrd + 1,
        )


def gen_mb_params_str(thrd):
    return thrd


def nf_blocks_declares(chain_pos, ch_len, thrds):
    """
    format ft block declares based on the position of middlebox in the chain, number of threads, and
    mb parameters
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param ch_len: the chain's length
    :param thrds: a number denoting the number of threads
    :return: formatted string of the ft_blocks
    """

    declares = []
    format_str = NF_BLOCK_FORMAT_STR

    for i in range(thrds):
        mb_p = gen_mb_params_str(i)
        if mb_p != '':
            mb_p = ', ' + str(mb_p)

        params = {
            QUEUE: i,
            MB_PARAMS: mb_p,
            DATA_SRC_IP: src_ip_filter(chain_pos, i)
        }

        declares.append(
            format_str.format(**params)
        )

    return '\n'.join(declares)


def links(thrds):
    """
    format link strings, each link is 'FromDevice... -> FTBlock -> ...ToDevice'
    :param thrds: a number denoting the number of threads
    :return: formatted string of links
    """
    fd_data_names = fd_data_names_list(thrds)
    td_data_names = td_data_names_list(thrds)
    nf_block_names = nf_block_names_list(thrds)

    format_str = LINK_FORMAT_STR

    ll = []
    for i in range(thrds):
        params = {
            QUEUE: i,
            'FROM_DATA_DEVICE_NAME': fd_data_names[i],
            'NF_BLOCK_NAME': nf_block_names[i],
            'TO_DATA_DEVICE_NAME': td_data_names[i],
        }
        ll.append(format_str.format(**params))

    return '\n'.join(ll)


def nf_block_def(chain_pos, mb, mb_params):
    """
    format a block declare
    :param chain_pos: a number denoting the position of middlebox in the chain
    :param mb_params: the list of parameter lists for MB's threads
    :return: formatted string of ft block
    """
    data_dev = '40'

    mb_params_str = ''
    if mb_params is not None and len(mb_params) > 0:
        mb_params_str = ', ' + ', '.join(mb_params)

    result = NF_BLOCK.format(**{
        SRC_IP_FILTER: src_ip_filter(chain_pos - 1),

        MB: mb,
        MB_PARAM: mb_params_str,

        DATA_SRC_MAC: dev_mac(chain_pos, data_dev),
        DATA_DST_MAC: dev_mac(chain_pos + 1, data_dev),
        DATA_DST_IP: dev_ip(chain_pos + 1, data_dev),
        DATA_SRC_NAME: dev_name(chain_pos),
        DATA_DST_NAME: dev_name(chain_pos + 1),
    })

    return result


def nf_click(ch_len, chain_pos, thrds, mb):
    """
    Click code for FTC
    :return: Click code in string
    """
    mb_ = None
    mb_params_ = None
    mb_params_vals_ = None

    if mb == COUNTER:
        mb_ = COUNTER_MB
        mb_params_ = COUNTER_MB_PARAMS

    elif mb == LB_MB:
        mb_ = BEAMER_MUX_MB
        mb_params_ = BEAMER_MUX_MB_PARAMS

    string_map = {
        SHARED_STATE_DECLARE: shared_state_declare(),

        NF_BLOCK_DEF: nf_block_def(chain_pos,
                                   mb_,
                                   mb_params_),

        FROM_DEVICE_DECLARES: from_dev_declares(chain_pos,
                                                thrds),

        TO_DEVICE_DECLARES: to_dev_declares(chain_pos,
                                            thrds),

        THREAD_SCHEDULES: thread_sched_declare(fd_names_list(chain_pos,
                                                             thrds)),

        NF_BLOCKS_DECLARES: nf_blocks_declares(chain_pos,
                                               ch_len,
                                               thrds),

        LINKS: links(thrds),
    }

    return NF.format(**string_map)


def generate(ch_len, thrds, mb):
    clicks = []
    for chain_pos in range(ch_len):
        clicks.append(nf_click(ch_len, chain_pos, thrds, mb))

    return clicks


def store(output_path, clicks):
    for i in range(len(clicks)):
        file_name = os.path.join(output_path, NF_OUTPUT_NAME_FORMAT_STR.format(i + 1))
        with open(file_name, 'w') as handle:
            handle.write(clicks[i])
