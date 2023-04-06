
#ifndef BUFFER_INDEX_H
	#define BUFFER_INDEX_H

	#include <d3d11.h>
	#include <vector>

	class IndexBuffer
	{
	public:
		IndexBuffer(ID3D11Device* device = nullptr, ID3D11DeviceContext* context = nullptr, std::size_t reserve = 50);
		~IndexBuffer();

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, unsigned int reserve = 50);
		void AddIndex(unsigned int index);
		void Clear();
		void Build();
		void Bind();

		unsigned int GetIndexCount();

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		std::vector<unsigned int> indices;
		ID3D11Buffer* buffer;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA subresource;
		bool is_built;
	};

#endif // !BUFFER_INDEX_H
