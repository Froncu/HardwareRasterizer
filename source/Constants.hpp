static constexpr uint32_t
WINDOW_WIDTH{ 640 },
WINDOW_HEIGHT{ 480 },
WINDOW_PIXEL_COUNT{ WINDOW_WIDTH * WINDOW_HEIGHT };

static constexpr float ASPECT_RATIO{ static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT };

constexpr char CONTROLS[]
{
	"--------\n"
	"CONTROLS:\n"
	"F2: Toggle Filtering Methods\n"
	"SCROLL: In-/decrement FOV angle\n"
	// ...
};