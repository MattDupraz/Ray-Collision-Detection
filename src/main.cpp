#include "app.h"
#include "ball.h"

int main()
{
	App app(500, 500, "Collision testing");

	app.add_ball(50, {60, 60}, {300, 200});
	app.add_ball(40, {400, 400}, {-150, -200});

	return app.start();
}
