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
local MAX_NODE <const> = 11
local ROPE_NODE_WIDTH <const> = 0.1
local ROPE_NODE_HEIGHT <const> = 0.1
local JOINT_STEP <const> = WORLD_WIDTH / (MAX_NODE - 1)
local MAX_MASS = 80
local initialized = false
local world = nil
local floor = nil
local rope_joints = table.create(MAX_NODE - 2, 0);
local rope_nodes = table.create(MAX_NODE, 0)
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

    local node_min = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 0)
    node_min:setCenter(0,1.0)
    node_min:setFriction(0.2)
    world:addBody(node_min)
    rope_nodes[#rope_nodes + 1] = node_min
    
    for i = 1, MAX_NODE - 2, 1 do
        print(i * 0.3 * 10)
        local node = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 0)
        node:setCenter(JOINT_STEP * i, 0)
        node:setFriction(0.2)
        world:addBody(node)
        rope_nodes[#rope_nodes + 1] = node
    end

    local node_max = playbox.body.new(ROPE_NODE_WIDTH, ROPE_NODE_HEIGHT, 0)
    node_max:setCenter(WORLD_WIDTH, 1.0)
    node_max:setFriction(0.2)
    world:addBody(node_max)
    rope_nodes[#rope_nodes + 1] = node_max
    
    local right_col = playbox.body.new(COLUMN_WIDTH, COLUMN_HEIGHT, COLUMN_MASS)
    right_col:setCenter(WORLD_WIDTH - (COLUMN_WIDTH/2), 2.5)
    right_col:setFriction(COLUMN_FRICTION)
    world:addBody(right_col)
    columns[#columns + 1] = right_col


    -- Create rope joints
    for i = 1, math.floor(MAX_NODE / 2) + 1, 1 do
        local x, y = rope_nodes[i]:getCenter();
        local joint = playbox.joint.new(rope_nodes[i], rope_nodes[i + 1], x, y)
        joint:setBiasFactor(0.3)
        joint:setSoftness(0.6)
        world:addJoint(joint)
        rope_joints[#rope_joints + 1] = joint
    end

    for i = MAX_NODE, (MAX_NODE / 2) + 2 , -1 do
        local x, y = rope_nodes[i]:getCenter();
        local joint = playbox.joint.new(rope_nodes[i], rope_nodes[i - 1], x, y)
        joint:setBiasFactor(0.3)
        joint:setSoftness(0.6)
        world:addJoint(joint)
        rope_joints[#rope_joints + 1] = joint
    end
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