

-- DO NOT MODIFY, THIS FILE IS GENERATED --
-- VERSION 0.0.2 --
---@meta
-- All functions related to "doing" stuff.

---@class ProtoLogic
---@field ship_self_destruct fun(): nil
---@field engine_set_throttle fun(throttle: number): nil
--- Set the torque for the ship to rotate.
---@field wheel_set_torque fun(x: number, y: number, z: number): nil
---@field runninglight_set_state fun(state: number): nil
---@field radar_set_angle fun(angle: number): nil
---@field radar_set_bearing fun(bearing: number): nil
---@field radar_set_elevation fun(elevation: number): nil
---@field radar_trigger fun(): nil
---@field gun0_set_bearing fun(bearing: number): nil
---@field gun0_set_elevation fun(elevation: number): nil
---@field gun0_set_fuse fun(fuse: number): nil
---@field gun0_trigger fun(): nil
---@field gun1_set_bearing fun(bearing: number): nil
---@field gun1_set_elevation fun(elevation: number): nil
---@field gun1_set_fuse fun(fuse: number): nil
---@field gun1_trigger fun(): nil
---@field gun2_set_bearing fun(bearing: number): nil
---@field gun2_set_elevation fun(elevation: number): nil
---@field gun2_set_fuse fun(fuse: number): nil
---@field gun2_trigger fun(): nil
---@field gun3_set_bearing fun(bearing: number): nil
---@field gun3_set_elevation fun(elevation: number): nil
---@field gun3_set_fuse fun(fuse: number): nil
---@field gun3_trigger fun(): nil
---@field debug_sphere_set fun(x: number, y: number, z: number, radius: number, r: number, g: number, b: number): nil
---@field debug_line_set fun(x1: number, y1: number, z1: number, x2: number, y2: number, z2: number, r: number, g: number, b: number): nil
protologic = nil
