
#ifndef ISERVICE_H
	#define ISERVICE_H

	class IService
	{
	public:
		virtual void Update() {}
		virtual bool Initialize() { return true; }
		virtual ~IService() {}
	};

#endif // !ISERVICE_H
