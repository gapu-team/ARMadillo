import typer
import utils

app              = typer.Typer()
SECTION_NAME     = "policy"

# Generate new section name
def get_next_section_name(config):
    new_section = SECTION_NAME
    if len(config.sections()) != 0:
        new_section += '_' + str(int(config.sections()[-1].split('-')[1]) + 1)
    else:
        new_section += '_1'

    return new_section

@app.command(help="Add a 5-tuple policy")
def policy(
    src_port: str = typer.Option(...,help="Packet source port"),
    dst_port: str = typer.Option(...,help="Packet destination port"),
    src_address: str = typer.Option(...,help="Packet source address"),
    dst_address: str = typer.Option(...,help="Packet source address"),
    protocol: str = typer.Option(...,help="Packet protocol"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = utils.policy_file_path

    config            = utils.get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'src_port', src_port)
    config.set(next_section_name, 'dst_port', dst_port)
    config.set(next_section_name, 'src_address', src_address)
    config.set(next_section_name, 'dst_address', dst_address)
    config.set(next_section_name, 'protocol', protocol)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    utils.save_to_file(config, filepath)

@app.command(help="Add a DNS policy")
def policy_dns(
    domain: str = typer.Option(...,help="domain to avoid e.g: --domain=Iran-online"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = utils.dns_policy_path

    config            = utils.get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'domain', domain)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    utils.save_to_file(config, filepath)

@app.command(help="Add a DPI policy")
def policy_dpi(
    string: str = typer.Option(...,help="string to avoid e.g: --string='alert tcp any any -> any any (msg:\"Youtube\"; flow:to_server; pcre:\"/.youtube./\"; sid:1; rev:1;)'"),
    enabled: bool = typer.Option(True, "--enabled/--disabled",help="If the policy enabled")):

    filepath = utils.dpi_policy_path

    config            = utils.get_config(filepath)
    next_section_name = get_next_section_name(config)
    config.add_section(next_section_name)
    config.set(next_section_name, 'string', string)
    config.set(next_section_name, 'enabled', str(int(enabled)))
    utils.save_to_file(config, filepath)

if __name__ == "__main__":
    app()