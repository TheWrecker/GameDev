
#ifndef PROXIES_H
	#define PROXIES_H

	#include <list>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class Proxy;
	class Presenter;

	class ProxyRender : public RenderBase
	{
	public:
		ProxyRender(Presenter* parent);
		~ProxyRender();

		void Render() override;
		void ShowProxies();
		void HideProxies();

		void AddProxy(Proxy* target);
		void RemoveProxy(Proxy* target);

	private:
		friend class Overlay;

		bool enabled;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_object_buffer;
		std::list<Proxy*> proxies;
	};

#endif // !PROXIES_H
