#include "app.h"
#include "ball.h"

int main()
{
	App app(500, 500, "Collision testing");

	app.add_ball({60, 60}, 50);
	app.add_ball({400, 400}, 40);

	return app.start();
}
