import typer
import configparser

app = typer.Typer()

policy_file_path = "./test.ini"
dns_policy_path  = "./dns.ini"
dpi_policy_path  = "./dpi.ini"

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

@app.command()
def policy(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = policy_file_path
    config   = get_config(filepath)
    config.remove_section(policy_id)
    save_to_file(config,filepath)

@app.command()
def policy_dns(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = dns_policy_path
    config   = get_config(filepath)
    config.remove_section(policy_id)
    save_to_file(config,filepath)

@app.command()
def policy_dpi(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = dpi_policy_path
    config   = get_config(filepath)
    config.remove_section(policy_id)
    save_to_file(config,filepath)


if __name__ == "__main__":
    app()