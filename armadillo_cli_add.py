import typer
import configparser

app              = typer.Typer()
policy_file_path = "./test.ini"
dns_policy_path  = "./dns.ini"
dpi_policy_path  = "./dpi.ini"
SECTION_NAME     = "policy"

# Return config parser from file
def get_config(file_path):
    config = configparser.ConfigParser()
    config.read(file_path)
    return config

# Generate new section name
def get_next_section_name(config):
    new_section = ""
    if len(config.sections()) != 0:
        new_section = SECTION_NAME+'-'+str(int(config.sections()[-1].split('-')[1])+1)
    else:
        new_section = SECTION_NAME+'-1'

    return new_section

# Save to file
def save_to_file(config,
                 file_path):
    with open(file_path, 'w') as configfile:
        config.write(configfile)

@app.command(help="Add a 5-tuple policy")
def policy(
    src_port: str = typer.Option(...,help="Packet source port"),
    dst_port: str = typer.Option(...,help="Packet destination port"),
    src_address: str = typer.Option(...,help="Packet source address"),
    dst_address: str = typer.Option(...,help="Packet source address"),
    protocol: str = typer.Option(...,help="Packet protocol"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = policy_file_path

    config            = get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'src_port', src_port)
    config.set(next_section_name, 'dst_port', dst_port)
    config.set(next_section_name, 'src_address', src_address)
    config.set(next_section_name, 'dst_address', dst_address)
    config.set(next_section_name, 'protocol', protocol)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    save_to_file(config, filepath)

@app.command(help="Add a DNS policy")
def policy_dns(
    domain: str = typer.Option(...,help="domain to avoid e.g: --domain=Iran-online"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = dns_policy_path

    config            = get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'domain', domain)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    save_to_file(config, filepath)

@app.command(help="Add a DPI policy")
def policy_dpi(
    string: str = typer.Option(...,help="string to avoid e.g: --string='alert tcp any any -> any any (msg:\"Youtube\"; flow:to_server; pcre:\"/.youtube./\"; sid:1; rev:1;)'"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = dpi_policy_path

    config            = get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'string', string)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    save_to_file(config, filepath)


if __name__ == "__main__":
    app()