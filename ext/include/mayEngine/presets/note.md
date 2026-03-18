# scene from json 格式

```json
{
    "class": "Scene",
    "initargs": {
        "size": {
            "width": 800,
            "height": 600
        }
    }
    "attributes": {
        "position": {
            "x": 0,
            "y": 0,
            "z": 0
        },
        "rotation": {
            "x": 0
        }
    },
    "components": [
        {
            "class": "RendSpriteComponent",
            "initargs": {
                "man": {
                    "class": "ResourceManager",
                    "initargs": {
                        "path": "...",
                        "key": "...",
                        "sign": "..."
                    }
                },
                "path": "..."
            },
            "inittype": "Manager",
            "attributes": {
                
            }
        },
        {
            "class": "LuaComponent",
            "initargs": {
                ""
            }
        }
    ]
}
```

