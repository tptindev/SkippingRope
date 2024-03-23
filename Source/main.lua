local graphics <const> = playdate.graphics
local geometry <const> = playdate.geometry
local sprite   <const> = playdate.sprite
local sys      <const> = playdate.system
local display  <const> = playdate.display

local WORLD_WIDTH <const> = 5.0
local WORLD_HEIGHT <const> = WORLD_WIDTH * 0.6 -- 0.6 => 240/400
local WORLD_PIXEL_SCALE <const> = 400.0 / WORLD_WIDTH
local FLOOR_WIDTH <const> = 24.0
local FLOOR_HEIGHT <const> = 0.5
local FLOOR_FRICTION <const> = 0.2
local FLOOR_MASS <const> = 0
local COLUMN_WIDTH <const> = 0.1
local COLUMN_HEIGHT <const> = 2.5
local COLUMN_MASS <const> = 0
local COLUMN_FRICTION <const> = 0.2
local ROPE_NODE_WIDTH <const> = 0.2
local ROPE_NODE_HEIGHT <const> = 0.1

local initialized = false
local world = nil
local floor = nil
local rope = nil
local rope_joints = table.create(3,0);
local rope_nodes = table.create(5, 0)
local node_center = nil;
local columns = table.create(2, 0)

function playdate.update()
    if not initialized then
        initialize()
    else
        local dt <const> = 1.0 / display.getRefreshRate()
        update(dt)
        draw()
    end
end

function initialize()
    initialized = true

    -- Setup game refresh rate
    display.setRefreshRate(30.0)

    -- Setup background color
    graphics.setBackgroundColor(graphics.kColorBlack)

    -- create world
    world = playbox.world.new(0.0, 9.81, 10)
    world:setPixelScale(WORLD_PIXEL_SCALE)

    -- create floor
    floor = playbox.body.new(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_MASS)
    floor:setCenter(WORLD_WIDTH / 2, WORLD_HEIGHT)
    floor:setFriction(FLOOR_FRICTION)
    world:addBody(floor)

    -- Create columns
    local left_col = playbox.body.new(COLUMN_WIDTH, COLUMN_HEIGHT, COLUMN_MASS)
    left_col:setCenter(COLUMN_WIDTH / 2, 2.5)
    left_col:setFriction(COLUMN_FRICTION)
    world:addBody(left_col)
    columns[#columns + 1] = left_col

    local right_col = playbox.body.new(COLUMN_WIDTH, COLUMN_HEIGHT, COLUMN_MASS)
    right_col:setCenter(WORLD_WIDTH - COLUMN_WIDTH / 2, 2.5)
    right_col:setFriction(COLUMN_FRICTION)
    world:addBody(right_col)
    columns[#columns + 1] = right_col

    local node_1 = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 80)
    node_1:setCenter(1, 0.1)
    node_1:setFriction(0.1)
    world:addBody(node_1)
    rope_nodes[#rope_nodes + 1] = node_1

    node_center = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 80)
    node_center:setCenter(2.5, 0.1)
    node_center:setFriction(0.1)
    world:addBody(node_center)
    rope_nodes[#rope_nodes + 1] = node_center
    
    local node_2 = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 80)
    node_2:setCenter(4, 0.1)
    node_2:setFriction(0.1)
    world:addBody(node_2)
    rope_nodes[#rope_nodes + 1] = node_2

    -- Create rope joints
    local rope_joint_0 = playbox.joint.new(left_col, node_1, 0, 1.5)
    rope_joint_0:setBiasFactor(0.2)
    rope_joint_0:setSoftness(1)
    world:addJoint(rope_joint_0)
    rope_joints[#rope_joints + 1] = rope_joint_0

    local rope_joint_3 = playbox.joint.new(right_col, node_2, WORLD_WIDTH, 1.5)
    rope_joint_3:setBiasFactor(0.2)
    rope_joint_3:setSoftness(1)
    world:addJoint(rope_joint_3)
    rope_joints[#rope_joints + 1] = rope_joint_3

    local x1, y1 = node_1:getCenter()
    local rope_joint_1 = playbox.joint.new(node_1, node_center, x1, y1)
    rope_joint_1:setBiasFactor(0.3)
    rope_joint_1:setSoftness(1)
    world:addJoint(rope_joint_1)
    rope_joints[#rope_joints + 1] = rope_joint_1

    local x2, y2 = node_2:getCenter()
    local rope_joint_2 = playbox.joint.new(node_2, node_center, x2, y2)
    rope_joint_2:setBiasFactor(0.3)
    rope_joint_2:setSoftness(1)
    world:addJoint(rope_joint_2)
    rope_joints[#rope_joints + 1] = rope_joint_2



end

function update(dt)
    world:update(dt)
end

function draw()
    graphics.clear(graphics.kColorWhite)
    graphics.setColor(graphics.kColorBlack)

    -- Draw flooring
    local floor_polygon = geometry.polygon.new(floor:getPolygon())
    floor_polygon:close()
    graphics.fillPolygon(floor_polygon)


    -- Draw columns
    for i, col in ipairs(columns) do
        local col_polygon = geometry.polygon.new(col:getPolygon())
        col_polygon:close()
        graphics.fillPolygon(col_polygon)
    end

    for i, node in ipairs(rope_nodes) do
        local node_polygon = geometry.polygon.new(node:getPolygon())
        node_polygon:close()
        graphics.fillPolygon(node_polygon)
    end

    -- Draw rope joints
    for i, joint in ipairs(rope_joints) do
        graphics.setStrokeLocation(graphics.kStrokeCentered)
        local _, _, px1, py1, x2, y2, _, _ = joint:getPoints()
        graphics.setDitherPattern(0.5)
        graphics.drawLine(x2, y2, px1, py1)
    end

    -- Draw FPS on device
    if not playdate.isSimulator then
        playdate.drawFPS(380, 15)
    end
end