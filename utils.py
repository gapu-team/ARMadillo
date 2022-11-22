import configparser
import os.path

armadillo_dir_path = os.path.join(os.path.expanduser("~"),".armadillo")
policy_file_path = armadillo_dir_path + "/policy.ini"
dns_policy_path = armadillo_dir_path + "/dns.ini"
dpi_policy_path = armadillo_dir_path + "/dpi.ini"
armadillo_pids_path = armadillo_dir_path + "/armadillo.pid"
parsed_policy = armadillo_dir_path + "/parsed_policy.csv"
parsed_dns = armadillo_dir_path + "/parsed_dns.csv"
parsed_dpi = armadillo_dir_path + "/parsed_dpi.csv"


# Return config parser from file
def get_config(file_path):
    config = configparser.ConfigParser()
    config.read(file_path)
    return config

# Save to file
def save_to_file(config,
                 file_path):
    with open(file_path, 'w') as configfile:
        config.write(configfile)

# Parsing file
def parse_file(rows,
               file_path):
    file = open(file_path, "w")
    file.write(rows)
    file.close()