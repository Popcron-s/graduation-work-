#pragma once

namespace _GRAPH{
	enum class TYPE{
		NONE = 0,
		STOP,
		PAUSE,
		PLAY
	};
	enum class INTERPOLATION{
		POINT,
		CLIFF,
		LINEAR,
		SQUARED,
		INVSQURED
	};
};
/*
namespace _OBJECT{
	enum TYPE{
		NONE = 0,
		Root,
		Material,
		Mesh,
		IndexedMesh,
		Collider,
		Sound,
		Animation
	};
};
*/
namespace _COLLISION{
	enum TYPE{
		Ignore,
		Overlap,
		Block
	};
}

namespace _PLATFORM{
	enum TYPE{
		Win32,
		Win64,
		Android,
		iOS
	};
}

namespace _INTERFACE{
	enum class GRAPHIC{
		OpenGL,
		DX9,
		DX11
	};

	enum class SOUND{
		XAudio2
	};
}