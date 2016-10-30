/*!
\file nsui_comp.h

\brief Header file for nsui_comp class

This file contains all of the neccessary declarations for the nsui_comp class.

\author Daniel Randle
\date November 23 2013
\copywrite Earth Banana Games 2013
*/

#ifndef NSUI_COMP_H
#define NSUI_COMP_H

#include <component/nscomponent.h>
#include <nsstring.h>
#include <nsrouter.h>

class nsmaterial;
class nsfont;
class nsui_material_comp;

class nsui_material_comp : public nscomponent
{
public:
	template <class PUPer>
	friend void pup(PUPer & p, nsui_material_comp & tc);

	nsui_material_comp();
	nsui_material_comp(const nsui_material_comp & rhs);

	virtual ~nsui_material_comp();

	virtual nsui_material_comp * copy(const nscomponent * comp_);

	virtual void init();

	virtual void pup(nsfile_pupper * p);

	nsui_material_comp & operator=(nsui_material_comp rhs_);

	uivec2 mat_shader_id;
	uivec2 mat_id;
	uivec2 border_mat_id;
	fvec4 border_size;
	fvec4 top_border_radius;
	fvec4 bottom_border_radius;
};

template <class PUPer>
void pup(PUPer & p, nsui_material_comp & tc)
{
	pup(p, tc.mat_shader_id, "mat_shader_id");
	pup(p, tc.mat_id, "mat_id");
	pup(p, tc.border_mat_id, "border_mat_id");
	pup(p, tc.border_size, "border_size");
	pup(p, tc.top_border_radius, "top_border_radius");
	pup(p, tc.bottom_border_radius, "bottom_border_radius");
}

class nsui_text_comp : public nscomponent
{
public:
	enum t_alignment
	{
		top_left,
		top_center,
		top_right,
		middle_left,
		middle_center,
		middle_right,
		bottom_left,
		bottom_center,
		bottom_right
	};
		
	template <class PUPer>
	friend void pup(PUPer & p, nsui_text_comp & tc);

	nsui_text_comp();
	nsui_text_comp(const nsui_text_comp & rhs);

	virtual ~nsui_text_comp();

	virtual nsui_text_comp * copy(const nscomponent * comp_);

	virtual void init();

	virtual void pup(nsfile_pupper * p);

	nsui_text_comp & operator=(nsui_text_comp rhs_);

	uivec2 text_shader_id;
	uivec2 font_id;
	uivec2 font_material_id;
	uivec4 margins;	
	nsstring text;
	t_alignment text_alignment;	
	std::vector<uint32> text_line_sizes;
};

template <class PUPer>
void pup(PUPer & p, nsui_text_comp::t_alignment & tc, const nsstring & str)
{
	uint32 ty = static_cast<uint32>(tc);
	pup(p,ty,str);
	tc = static_cast<nsui_text_comp::t_alignment>(ty);
}

template <class PUPer>
void pup(PUPer & p, nsui_text_comp & tc)
{
	pup(p, tc.text_shader_id, "text_shader_id");
	pup(p, tc.font_id, "font_id");
	pup(p, tc.font_material_id, "font_mat_id");
	pup(p, tc.margins, "margins;");
	pup(p, tc.text, "text");
	pup(p, tc.text_alignment, "text_alignment;");
	pup(p, tc.text_line_sizes, "text_line_sizes");
}

class nsui_text_input_comp : public nscomponent
{
public:		
	template <class PUPer>
	friend void pup(PUPer & p, nsui_text_input_comp & tc);

	nsui_text_input_comp();
	nsui_text_input_comp(const nsui_text_input_comp & rhs);

	virtual ~nsui_text_input_comp();

	virtual nsui_text_input_comp * copy(const nscomponent * comp_);

	virtual void init();

	virtual void pup(nsfile_pupper * p);

	nsui_text_input_comp & operator=(nsui_text_input_comp rhs_);

	float cursor_blink_rate_ms;
	uint32 cursor_pixel_width;
	fvec4 cursor_color;
	uivec2 cursor_offset;
};

template <class PUPer>
void pup(PUPer & p, nsui_text_input_comp & tc)
{
	pup(p, tc.cursor_blink_rate_ms, "cursor_blink_rate_ms");
	pup(p, tc.cursor_pixel_width, "cursor_pixel_width");
	pup(p, tc.cursor_color, "cursor_color");
	pup(p, tc.cursor_offset, "cursor_offset");
}

#endif
