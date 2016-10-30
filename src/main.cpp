#include <opengl/nsgl_driver.h>
#include <opengl/nsgl_window.h>

#include <nsengine.h>

#include <asset/nsplugin.h>
#include <asset/nsscene.h>
#include <asset/nsplugin_manager.h>
#include <asset/nsinput_map.h>

#include <system/nscamera_system.h>

#include <component/nscam_comp.h>


void setup_input_map(nsplugin * plg);


int main()
{
    nsengine e;
    e.init(new nsgl_driver);
	
    nsgl_window wind(ivec2(800,600), "Basic Test");
    viewport * vp = wind.vid_context()->insert_viewport("main_view",fvec4(0.0f,0.0f,1.0f,1.0f));
	
    nsplugin * plg = nsep.create<nsplugin>("most_basic_test");
    plg->enable(true);
    plg->create_global_input_map("input_map", "global_ctxt");
	
    vp->camera = plg->create_camera("scenecam", 60.0f, uivec2(400, 400), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    nsentity * dirl = plg->create_dir_light("dirlight", 1.0f, 0.0f,fvec3(1.0f,1.0f,1.0f),true,0.5f,2);

    nsscene * new_scene = plg->create<nsscene>("new_scene");
    new_scene->enable(true);	
    new_scene->set_bg_color(fvec3(0.7f, 0.7f, 1.0f));
	new_scene->add(vp->camera, fvec3(0,0,-20));
    new_scene->add(dirl, fvec3(5.0f, 5.0f, -20.0f), orientation(fvec4(1,0,0,20.0f)));
    nse.set_active_scene(new_scene);
	
    e.start();
    while (e.running())
    {
        nse.update();

        wind.vid_context()->render(nse.active_scene());
        wind.update();
		window_poll_input();

        if (!wind.is_open())
            e.stop();
    }

    nse.release();
    return 0;
}

