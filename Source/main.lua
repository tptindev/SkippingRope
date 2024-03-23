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

local initialized = false
local world = nil
local floor = nil
local rope = nil
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
    display.setRefreshRate(8.0)

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

    -- Draw FPS on device
    if not playdate.isSimulator then
        playdate.drawFPS(380, 15)
    end
end