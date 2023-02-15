

#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../elements/buffer_index.h"
#include "../scene.h"
#include "../presenter.h"

#include "render_base.h"

RenderBase::RenderBase(Scene* scene)
	:scene(scene), presenter(scene->GetPresenter())
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	camera = scene->GetActiveCamera();
}

RenderBase::~RenderBase()
{
}

void RenderBase::Update()
{
}
