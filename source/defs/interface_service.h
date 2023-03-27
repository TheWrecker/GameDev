
#ifndef ISERVICE_H
	#define ISERVICE_H

	class IService
	{
	public:
		virtual ~IService() {}

		virtual void Update() {}

		virtual bool Initialize()
		{ 
			return true;
		}
	};

#endif // !ISERVICE_H
