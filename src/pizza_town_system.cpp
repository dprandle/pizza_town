#include <pizza_town_system.h>
#include <nsengine.h>
#include <asset/nsplugin.h>
#include <asset/nsplugin_manager.h>
#include <asset/nsscene.h>
#include <component/nscam_comp.h>
#include <component/nsrender_comp.h>
#include <component/nssel_comp.h>
#include <opengl/nsgl_window.h>
#include <opengl/nsgl_driver.h>
#include <asset/nsmaterial.h>
#include <asset/nstexture.h>
#include <component/nslight_comp.h>
#include <nstimer.h>
#include <system/nsaudio_system.h>
#include <asset/nsaudio_clip.h>
#include <component/nsaudio_source_comp.h>

#include <component/nsanim_comp.h>

#include <component/nstform_comp.h>

pizza_town_system::pizza_town_system():
	nssystem(type_to_hash(pizza_town_system)),
	wind(nullptr),
	viewp(nullptr)
{}
	
pizza_town_system::~pizza_town_system()
{}

void pizza_town_system::init()
{
	wind = new nsgl_window(ivec2(1920,1080), "Pizza Town V1.0");
    viewp = wind->vid_context()->insert_viewport("main_view",fvec4(0.0f,0.0f,1.0f,1.0f));

	viewp->m_fog_nf.x = 2000;
	viewp->m_fog_nf.y = 3000;

	nsplugin * plg = nsep.create<nsplugin>("pizza_town");
    plg->enable(true);

	bg = plg->create_sprite("pizza_town_bg", "./import/pizza_town_store.png", true, true);
	phone = plg->create_sprite("phone", "./import/phone.png", true, true);

	viewp->camera = plg->create_camera("main_cam",
							   fvec2(-1920/2,1920/2),
							   fvec2( 1080/2,-1080/2),
							   fvec2(1, -2000));
	
    plg->create_global_input_map("input_map", "global_ctxt");
		
    nsentity * dirl = plg->create_dir_light("dirlight", 1.0f, 0.0f,fvec3(1.0f,1.0f,1.0f),true,0.5f,2);
    scn = plg->create<nsscene>("scn");

    nsaudio_source_comp * src = phone->create<nsaudio_source_comp>();
    source_obj * sobj = new source_obj;
    sobj->buffer = plg->load<nsaudio_clip>("./import/phone_ring.wav", true);
    sobj->buffer->read_data();
    sobj->gain = 10;
    sobj->init();
    src->sources.push_back(sobj);




	scn->enable(true);
    scn->set_bg_color(fvec3(0.7f, 0.7f, 1.0f));
	scn->add(viewp->camera, fvec3(0,0,0));
    scn->add(dirl, fvec3(5.0f, 5.0f, -20.0f), orientation(fvec4(1,0,0,0.0f)));
	scn->add(bg, fvec3(0,0,5), orientation(fvec4(0,0,1,180)));
    scn->add(phone, fvec3(1706-1920/2,1080 - 581-1080/2,4), orientation(fvec4(0,0,1,180)));
    nse.set_active_scene(scn);

	timer = new nstimer;
	anim_phone = false;

    nse.system<nsaudio_system>()->units_per_meter = 100;
}

void pizza_town_system::setup_input_map(nsinput_map * im, const nsstring & gctxt)
{
	
}

void pizza_town_system::release()
{
		
}

void pizza_town_system::update()
{
    float angle = (rand() % 1000) / 70.0f + 180.0f;
    static fquat start;
    if (int(nse.timer()->elapsed()*10) % 30 == 0 && !anim_phone)
	{
		timer->start();
		anim_phone = true;
        nstform_comp * tfc = phone->get<nstform_comp>();
        start = tfc->world_orientation();
        nse.event_dispatch()->push<audio_play_event>(uivec3(phone->full_id(),0), 0);
	}
	if (anim_phone)
	{
		timer->update();
		nstform_comp * tfc = phone->get<nstform_comp>();
        tfc->set_world_orientation(orientation(fvec4(0,0,1,angle)));
        if (timer->elapsed() >= 1.0f)
		{
			anim_phone = false;
            tfc->set_world_orientation(start);
		}
	}
}

int32 pizza_town_system::update_priority()
{
	return PIZZA_TOWN_UPDATE_PR;
}

