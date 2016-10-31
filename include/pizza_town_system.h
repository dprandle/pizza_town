#include <system/nssystem.h>

#define PIZZA_TOWN_UPDATE_PR 150000

class nstimer;
class nsgl_window;
struct viewport;
class nsentity;
class nsscene;
class nsaudio_clip;

class pizza_town_system : public nssystem
{
  public:
	
	pizza_town_system();
	
	~pizza_town_system();

	void init();

	void setup_input_map(nsinput_map * im, const nsstring & gctxt);

	void release();

	void update();

	int32 update_priority();

	nsgl_window * wind;
	viewport * viewp;
	nsentity * phone;
	nsentity * bg;
	nsscene * scn;
	nstimer * timer;
    nsaudio_clip * sound;
	bool anim_phone;
};

