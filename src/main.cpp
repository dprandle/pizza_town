
#include <opengl/nsgl_driver.h>
#include <opengl/nsgl_window.h>

#include <nsengine.h>

#include <asset/nsplugin.h>
#include <asset/nsscene.h>
#include <asset/nsplugin_manager.h>
#include <asset/nsinput_map.h>

#include <system/nscamera_system.h>

#include <component/nscam_comp.h>

#include <pizza_town_system.h>

int main()
{
    nsengine e;
	e.register_system<pizza_town_system>("pizza_town_system");
    e.init(new nsgl_driver);
	pizza_town_system * sys = nse.system<pizza_town_system>();
	
    e.start();
    while (e.running())
    {
        nse.update();
        sys->wind->vid_context()->render(nse.active_scene());
        sys->wind->update();
		window_poll_input();
        if (!sys->wind->is_open())
            e.stop();
    }

    nse.release();
    return 0;
}

