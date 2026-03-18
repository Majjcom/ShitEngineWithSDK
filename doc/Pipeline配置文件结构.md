# Pipeline配置文件结构

## Normal Pipeline

### 首先是Shaders:

```json
{
    "a_shader": {
        "vertex": "/shader/a.vs",
        "fragment": "/shader/a.fs",
        "id": 0,
        "[type]": "[Normal]"
    },
    "b_shader": {
        "vertex": "/shader/b.vs",
        "fragment": "/shader/b.fs"
    },
    "c_shader": {
        "compute": "/shader/c.comp",
        "type": "Compute"
    }
}
```

### 其次是Bunches:

```json
{
    "a_bunch": {
        "clear_color": {
            "r": 1.0,
            "g": 1.0,
            "b": 1.0,
            "a": 1.0
        },
        "rank": 0,
        "target": "OutBunch | [mpurl]",
        "out_shader": "a_shader@/pipeline/shader.json",
        "buffers": [
            {
                "shader": "b_shader@/pipeline/shader.json",
                "type": "Linear | Nearest"
            },
            {
                "shader": "c_shader@/pipeline/shader.json",
                "type": "Linear | Nearest",
                "[size]": {
                    "width": 800,
                    "height": 600
                },
                "[buffer_wrap]": {
                    "S": "Repeat|MirroredRepeat|ClampToEdge|ClampToBorder",
                    "T": "Repeat|MirroredRepeat|ClampToEdge|ClampToBorder"
                },
                "[buffer_storage_type]": "UnsignedByte|Float",
                "[blend]": false
            }
        ],
        "[in_shaders]": {
            "texture": "a_shader@/pipeline/shader.json",
            "shape": "d_shader@/pipeline/shader.json"
        },
        "buffer_type": "Linear | Nearest",
        "[buffer_wrap]": {
            "S": "Repeat|MirroredRepeat|ClampToEdge|ClampToBorder",
            "T": "Repeat|MirroredRepeat|ClampToEdge|ClampToBorder"
        },
        "[buffer_size]": {
            "width": 800,
            "height": 600
        },
        "[buffer_storage_type]": "UnsignedByte|Float",
        "blend": false
    }
}
```

### 然后是Pipeline:

```json
{
    "in_bunches": [
        "a_bunch@/pipeline/bunch.json",
        "b_bunch@/pipeline/bunch.json
    ],
    "side_pipelines": [
        "mpurl..."
    ],
    "back_color": {
        "r": 1.0,
        "g": 1.0,
        "b": 1.0,
        "a": 1.0
    },
    "size": {
        "width": 800,
        "height": 600
    },
    "[default_bunch_id]": 0,
    "[default_side_id]": -1,
    "[preload_shaders]": [
        "mpurl..."
    ]
}
```

## Side Pipeline

### pipeline:

```json
{
	"name": {
        "out_bunch": "mpurl...",
        "in_bunches": [
        	"mpurl..."
        ],
    	"back_color": {
            "r": 1.0,
            "g": 1.0,
            "b": 1.0,
            "a": 1.0
        },
        "size": {
            "width": 800,
            "height": 600
        },
        "[update_method]": "Always|Sleep|Manual",
        "rank": 0,
        "[preload_shaders]": [
            "mpurl"
        ]
    }
}
```

### side bunch:

```json
{
    "bunch_name": {
        "clear_color": {
            "r": 1.0,
            "g": 1.0,
            "b": 1.0,
            "a": 1.0
        },
        "[id]": 0,
        "rank": 0,
        "target": "OutBunch | [mpurl]",
        "out_shader": "mpurl",
        "buffers": [
            {
                "shader": "b_shader@/pipeline/shader.json",
                "type": "Linear | Nearest"
            },
            {
                "shader": "c_shader@/pipeline/shader.json",
                "type": "Linear | Nearest",
                "[size]": {
                    "width": 800,
                    "height": 600
                },
                "[buffer_storage_type]": "UnsignedByte|Float"
            }
        ],
        "[in_shaders]": {
            "texture": "a_shader@/pipeline/shader.json",
            "shape": "d_shader@/pipeline/shader.json"
        },
        "buffer_type": "Linear | Nearest",
        "[buffer_size]": {
            "width": 800,
            "height": 600
        },
        "[buffer_storage_type]": "UnsignedByte|Float",
        "blend": false
    }
}
```

