
#ifndef IDRAWABLE_H
	#define IDRAWABLE_H

	class IDrawable
	{
	public:
		virtual void Draw() = 0;
		virtual void Update() {}
	};

#endif // !IDRAWABLE_H
