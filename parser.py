import configparser
import subprocess
import utils

def parsed_policy():
    config = utils.get_config(utils.policy_file_path)
    rows = ""
    for section in config.sections():
        if config[section]['enabled']:
            row=config[section]['src_address'] + "," + \
                config[section]['dst_address'] + "," + \
                config[section]['src_port'] + "," + \
                config[section]['dst_port'] + "," + \
                config[section]['protocol'].lower()

            if rows == "":
                rows = row
            else:
                rows = rows + "\n" + row

    utils.parse_file(rows, utils.parsed_policy)

def parsed_dns():
    config = utils.get_config(utils.dns_policy_path)
    rows = ""
    for section in config.sections():
        if config[section]['enabled']:
            row=config[section]['domain']

            if rows == "":
                rows = row
            else:
                rows = rows + "\n" + row

    utils.parse_file(rows, utils.parsed_dns)

def parsed_dpi():
    config = utils.get_config(utils.dpi_policy_path)
    rows = ""
    for section in config.sections():
        if config[section]['enabled']:
            row=config[section]['string']

            if rows == "":
                rows = row
            else:
                rows = rows + "\n" + row

    utils.parse_file(rows, utils.parsed_dpi)

def main():
   parsed_policy()
   parsed_dpi()
   parsed_dns()

if __name__ == "__main__":
    main()
    #armadillo = subprocess.Popen(["C:\\Users\\doron\\ARMadillo\\parser\\main.exe", "a", "b"], stdout=subprocess.PIPE)