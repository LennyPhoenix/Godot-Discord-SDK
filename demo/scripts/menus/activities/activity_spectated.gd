extends PanelContainer

onready var spectate_secret_line_edit := $VBoxContainer/MarginContainer/SpectateSecret/LineEdit


func _ready() -> void:
	var _err = DiscordManager.connect("initialised", self, "_on_discord_manager_initialised")


func _on_discord_manager_initialised() -> void:
	DiscordManager.activities.connect("activity_spectate", self, "_on_activity_spectate")


func _on_activity_spectate(spectate_secret: String) -> void:
	spectate_secret_line_edit.text = spectate_secret
