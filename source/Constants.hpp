static constexpr uint32_t
WINDOW_WIDTH{ 640 },
WINDOW_HEIGHT{ 480 },
WINDOW_PIXEL_COUNT{ WINDOW_WIDTH * WINDOW_HEIGHT };

static constexpr float ASPECT_RATIO{ static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT };

constexpr char CONTROLS[]
{
	"--------\n"
	"CONTROLS:\n"
	"F4: Toggle Filtering Mode\n"
	"F5: Toggle Rotation\n"
	"F6: Toggle Using Normal Texture\n"
	"F7: Toggle Fire Rendering\n"
	"SCROLL: In-/decrement FOV angle\n"
	// ...
};