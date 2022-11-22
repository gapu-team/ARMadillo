import typer
import utils
from tabulate import tabulate

app = typer.Typer()

# Return all rows in array
def get_rows(config):
    table = []

    for section in config.sections():
        row = []
        for key, val in config.items(section):
            row.append(val)
        table.append(row)

    return table

@app.command(help="list all policies")
def policy():
    filepath = utils.policy_file_path
    config   = utils.get_config(filepath)
    table = get_rows(config)
    print(tabulate(table, headers=["Source Port", "Destination Port", "Source Address", "Destination Address", "Protocol", "Enabled"]))

@app.command(help="list all dns policies")
def policy_dns():
    filepath = utils.dns_policy_path
    config   = utils.get_config(filepath)
    table = get_rows(config)
    print(tabulate(table, headers=["Domain", "Enabled"]))

@app.command(help="list all dpi policies")
def policy_dpi():
    filepath = utils.dpi_policy_path
    config   = utils.get_config(filepath)
    table = get_rows(config)
    print(tabulate(table, headers=["String", "Enabled"]))

if __name__ == "__main__":
    app()