extends ScrollContainer


signal switch_to(menu)


func _on_users_button_up() -> void:
	emit_signal("switch_to", Main.Menu.USERS)


func _on_images_button_up() -> void:
	emit_signal("switch_to", Main.Menu.IMAGES)


func _on_activities_button_up() -> void:
	emit_signal("switch_to", Main.Menu.ACTIVITIES)


func _on_relationships_button_up() -> void:
	emit_signal("switch_to", Main.Menu.RELATIONSHIPS)


func _on_networking_button_up() -> void:
	emit_signal("switch_to", Main.Menu.NETWORKING)
