import typer
import utils

app = typer.Typer()

@app.command()
def policy(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = utils.policy_file_path
    config   = utils.get_config(filepath)
    config.remove_section(policy_id)
    utils.save_to_file(config,filepath)

@app.command()
def policy_dns(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = utils.dns_policy_path
    config   = utils.get_config(filepath)
    config.remove_section(policy_id)
    utils.save_to_file(config,filepath)

@app.command()
def policy_dpi(policy_id: str = typer.Option(...,help="The policy id to delete e.g: --policy-id=policy-1")):
    filepath = utils.dpi_policy_path
    config   = utils.get_config(filepath)
    config.remove_section(policy_id)
    utils.save_to_file(config,filepath)


if __name__ == "__main__":
    app()