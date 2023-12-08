local protolua = require "protolua"


function init()
end

function tick()
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~")

	for i=1,protologic.radar_get_contact_count() do
		local contact = protologic.radar_get_contact_info(i-1)
		print(string.format("%d: Target %d %s ~ %g @ %g, %g, %g", i, contact.id, protolua.TargetTypes[contact.type], contact.signal_strength, contact.x, contact.y, contact.z))
	end

	protologic.radar_set_angle(90);
	protologic.radar_set_bearing(-90);
	protologic.radar_set_elevation(0);
	protologic.radar_trigger();
end
