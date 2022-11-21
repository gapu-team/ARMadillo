import typer
import configparser
from tabulate import tabulate

app = typer.Typer()

policy_file_path = "./test.ini"
dns_policy_path  = "./dns.ini"
dpi_policy_path  = "./dpi.ini"

# Return config parser from file
def get_config(file_path):
    config = configparser.ConfigParser()
    config.read(file_path)
    return config

# Return all rows in array
def get_rows(config):
    table = []

    for section in config.sections():
        row = []
        for key, val in config.items(section):
            row.append(val)
        table.append(row)

    return table

@app.command()
def policy():
    filepath = policy_file_path
    config   = get_config(filepath)
    table = get_rows(config)
    print(tabulate(table,headers=["Source Port", "Destination Port", "Source Address", "Destination Address", "Protocol", "Enabled"]))

@app.command()
def policy_dns():
    filepath = dns_policy_path
    config   = get_config(filepath)
    table = get_rows(config)
    print(tabulate(table,headers=["Domain", "Enabled"]))

@app.command()
def policy_dpi():
    filepath = dpi_policy_path
    config   = get_config(filepath)
    table = get_rows(config)
    print(tabulate(table,headers=["String", "Enabled"]))


if __name__ == "__main__":
    app()