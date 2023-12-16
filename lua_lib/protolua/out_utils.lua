-- Special output for sim running in protolua
local OutUtils = {}
OutUtils.ENABLED = true


function OutUtils.fileAppend(name, data)
	if not OutUtils.ENABLED then return end
	for line in (data .. "\n"):gmatch("(.-)\n") do
		print(("__PROTOLUA__:fileAppend:%s:%s"):format(name, line))
	end
end

function OutUtils.csvAppend(name, ...)
	if not OutUtils.ENABLED then return end
	local values = {...}
	for i, v in ipairs(values) do
		values[i] = tostring(v)
	end
	OutUtils.fileAppend(name, table.concat(values, ","))
end


return OutUtils
