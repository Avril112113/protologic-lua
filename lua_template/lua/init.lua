local protolua = require "protolua"


function init()
end

function tick()
	for i=1,protologic.radar_get_target_count() do
		local type = protologic.radar_get_target_type(i-1)
		local dist = protologic.radar_get_target_distance(i-1)
		print(string.format("%d: Target %s @ %gm", i, protolua.TargetTypes[type], dist))
	end

	protologic.radar_set_angle(90);
	protologic.radar_set_bearing(-90);
	protologic.radar_set_elevation(0);
	protologic.radar_trigger();
end
