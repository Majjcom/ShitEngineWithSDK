# Lua映射列表

## Vec3

- new() -> Vec3
- new(table) -> Vec3
- new(x: number) -> Vec3
- new(x: number, y: number) -> Vec3
- new(x: number, y: number, z: number) -> Vec3
- :getx() -> number
- :gety() -> number
- :getz() -> number
- :setx(x: number)
- :sety(y: number)
- :setz(z: number)
- :set(v: Vec3)
- :set(x: number, y: number, z: number)
- :rotate_to(other: Vec3) -> Vec3
- :transform(angles: Vec3, move: Vec3, scale: Vec3) -> Vec3
- :mod() -> number
- :\_\_unm() -> Vec3
- :normalized() -> Vec3
- :length() -> number
- getAngle(from: Vec3, to: Vec3) -> number
- getLineAngle(from: Vec3, to: Vec3) -> number
- angleToDegree(x: number) -> number
- :\_\_add(other: Vec3) -> Vec3
- :\_\_sub(other: Vec3) -> Vec3
- :\_\_mul(x: number) -> Vec3
- :\_\_mul(x: Vec3) -> number
- :\_\_eq(other: Vec3) -> boolean
- :\_\_tostring() -> string



## ScreenManager

- isWindowFocused() -> boolean
- isCursorInWindow() -> boolean
- setTransparency(yesno: boolean)
- setWindowAlpha(yesno: boolean)
- setViewSize(width: number, height: number)
- setBackColor(r: number, g: number, b: number, a: number)
- getScreenWidth() -> integer
- getScreenHeight() -> integer
- getMonitorInfo() -> width: integer, height: integer, frame_rate: integer
- setDecorated(yesno: boolean)
- setMaximized()
- setFullScreen()
- setWindowed()
- setFloating(yesno: boolean)
- getWindowPos() -> Vec3
- setWindowPos(p: Vec3)
- setWindowSize(s: Vec3)
- setTextureFilterNearest(yesno: boolean)
- setWindowTitle(title: string)
- setWindowLimited(yesno: boolean)
- setResizeCallback(callable: Callable)
- MessageBoxType
  - INFO
  - WARNING
  - ERROR
- showMessageBox(type: MessageBoxType(integer), title: string, message: string)
- getDesktopRect(id: integer) -> width: integer, height: integer



## InputManager

- checkKeyState(key: integer) -> KeyState
- getCursorPos() -> Vec3
- checkMouseButton(mouse: integer) -> KeyState
- getTouchFingers() -> { }
- .KeyState.RELEASE = 0
- .KeyState.PRESS = 1
- .KeyState.REPEAT = 2
- .MouseButton.LEFT = 0
- .MouseButton.RIGHT = 1
- .MouseButton.MIDDLE = 2



## ResourceManager

- new(path: string, key: string) -> ResourceManager
- new(path: string, key: string, pub_auth_key: string) -> ResourceManager
- :getResource(path: string) -> string(byte)
- :getResourceSize(path: string) -> integer
- :getLoadError() -> boolean
- :getPath() -> string
- :getAllResourcesLocation() -> [string]



## MayText

- new(man: ResourceManager, path: string) -> MayText
- :load_char(text: string, size: integer) -> table(MayTextData)
- :load_char(text: integer, size: integer) -> table(MayTextData)
- :load_text(text: string, size: integer) -> table(MayTextData, ...)



## Engine

- getRunningScene() -> Scene
- quickReRank()
- setIgnoreDt(yesno: boolean)
- getIgnoreDt() -> boolean
- setRankBlock(yesno: boolean)
- setGarbageCleanRate(rate: integer)
- setGarbageCleanRateType(type: integer)
- getGarbageCleanRateType() -> integer
- requireGC()
- setShouldCheckUselessNodes(yesno: boolean)
- runWithScene(scene: Scene)
- rankChanged()
- setFrameRate(rate: integer)
- getFrameRate() -> integer
- loadPipeline(man: ResourceManager, path: string) -> integer
- usePipeline(id: integer)
- getPipeline(id: integer) -> Pipeline
- getCurrentPipeline() -> Pipeline
- getClassesInPool() -> { string }
- exit()



## mlog

- \_\_call(any: Any, ...)
- t(any: Any, ...)
- d(any: Any, ...)
- i(any: Any, ...)
- w(any: Any, ...)
- e(any: Any, ...)
- Level
  - Trace
  - Debug
  - Info
  - Warn
  - Error
  - Off
- setLevel(level: Level(integer))



## utils

- convertPos(pos: Vec3, scene: Scene) -> Vec3
- calculateFixedSize(primary: Vec3, target: Vec3) -> Vec3
- tableToJson(table: table) -> string
- byteColor(color: Vec3) -> Vec3



## Pipeline

- :setLock(width: boolean, height: boolean)
- :getLock() -> boolean, boolean
- :setView(width: integer, height: integer)
- :getView() -> integer, integer
- :setBunchSize(mpurl: string, size: Vec3)
- :setSize(size: Vec3)
- :setSideBunchSize(muprl: string, size: Vec3)
- :setSideSize(id: integer, size: Vec3)
- :setUniform(id: integer, location: string, val: Any)
- :getSprite(side_id: integer) -> BunchSprite
- setTextureBinding(id: integer, sp: SpriteBase)
- :setTextureBinding(sprite_id: integer, id: integer, sp: SpriteBase)
- :getShaderById(id: integer) -> Shader
- .SidePipelineUpdateMethod
  - Always
  - Manual
  - Sleep
- :setSidePipelineUpdateMethod(id: integer, method: SidePipelineUpdateMethod(integer))
- :getSidePipelineUpdateMethod(id: integer) -> SidePipelineUpdateMethod(integer)
- :updateSidePipeline(id: integer)
- :clearSidePipeline(id: integer)
- :getDefaultBunchId() -> integer
- :getDefaultSideId() ->integer



## Ref

- :enable()
- :disable()
- :destroy()
- :isEnabled() -> boolean
- :isDestroyed() -> boolean
- :getClassName() -> string
- :cast(t: table) -> table



## Node: Ref

- create() -> Node
- quickCreate(parent: Node, rank: integer) -> Node
- \_\_create_from_ptr(ud: userdata, record: boolean) -> Node
- :getRoot() -> Scene
- :setParent(parent: Node)
- :setRank(rank: integer)
- :getRank() -> integer
- :getParent() -> Node
- :getPosition() -> Vec3
- :getPositionX() -> number
- :getPositionY() -> number
- :getPositionZ() -> number
- :setPosition(p: Vec3)
- :setPositionX(x: number)
- :setPositionY(y: number)
- :setPositionZ(z: number)
- :getRotation() -> Vec3
- :getRotationX() -> number
- :getRotationY() -> number
- :getRotationZ() -> number
- :setRotation(r: Vec3)
- :setRotationX(x: number)
- :setRotationY(y: number)
- :setRotationZ(z: number)
- :getScale() -> Vec3
- :getScaleX() -> number
- :getScaleY() -> number
- :getScaleZ() -> number
- :setScale(s: Vec3)
- :setScaleX(x: number)
- :setScaleY(y: number)
- :setScaleZ(z: number)
- :getAnchor() -> Vec3
- :getAnchorX() -> number
- :getAnchorY() -> number
- :getAnchorZ() -> number
- :setAnchor(a: Vec3)
- :setAnchorX(x: number)
- :setAnchorY(y: number)
- :setAnchorZ(z: number)
- :setQuickRank(rank: integer)
- :update(dt: number)
- :onEnable()
- :onEnter()
- :onDestroy()
- :addComponent(component: Component) -> integer
- :addComponentQueued(component: Component) -> integer
- :getComponent(id: integer) -> Component
- :getComponents() -> { Component }
- :transformToWorldPoint(p: Vec3) -> Vec3
- :transformToWorldRotation(r: Vec3) -> Vec3
- :\_\_get_table_index() -> integer
- :\_\_get_real_table() -> Node



## Camera: Node

- create() -> Camera
- \_\_craete_from_ptr(ud: userdata, record: boolean) -> Camera
- :transform(p: Vec3) -> Vec3



## Scene: Node

- create(size: Vec3) -> Scene
- \_\_create_from_ptr(ud: userdata, record: boolean) -> Scene
- :setMianCamera(cam: Camera)
- :getMainCamera() -> Camera
- :setSize(size: Vec3)
- :getSize() -> Vec3
- :setLock(width: boolean, height: boolean)
- :getLock() -> boolean, boolean



## RigidBodyNode: Node

- create() -> RigidBodyNode
- quickCreate() -> RigidBodyNode
- \_\_create_from_ptr() -> RigidBodyNode
- :rigid_init()
- :bindShape(shape: PhysicsShapeBase)
- :getRigidBodyType() -> integer(RigidBodyNode.RigidBodyType)
- :setRigidBodyType(type: integer(RigidBodyNode.RigidBodyType))
- :getLinearVelocity() -> Vec3
- :setLinearVelocity(velocity: Vec3)
- :getAngularVelocity() -> number
- :setAngularVelocity(velocity: number)
- :getLinearDamping() -> number
- :setLinearDamping(damping: number)
- :getAngularDamping() -> number
- :setAngularDamping(damping: number)
- :getAwake() -> boolean
- :setAwake(awk: boolean)
- :applyForce(force: Vec3, point: Vec3, wake: boolean)
- :applyTorque(torque: number, wake: boolean)
- :applyLinearImpulse(impluse: Vec3, point: Vec3, wake: boolean)
- // TODO



## SpriteBase

- :getId() -> integer
- :getWidth() -> number
- :getHeight() -> number
- :getError() -> boolean



## SharedSprite: SpriteBase

- new(man: ResourceManager, path: string) -> SharedSprite
- new(man: ResourceManager, path: string, wraps: string, wrapt: string) -> SharedSprite
- new(data: string(byte), size: integer, width: integer, height: integer) -> SharedSprite
- new(data: string(byte), size: integer, width: integer, height: integer, wraps: string, wrapt: string) -> SharedSprite
- \_\_create_from_ptr(ud: userdata, record: boolean) -> SharedSprite



## BunchSprite: SpriteBase

- \_\_new_ptr(ud: userdata) -> BunchSprite
- new(id: integer) -> BunchSprite
- :setSize(width: number, height: number)



## PlaceholderSprite: SpriteBase

- new() -> PlaceholderSprite
- :setSize(width: number, height: number)



## Component: Ref

- \_\_create_from_ptr(ud: userdata, record: boolean) -> Component
- :setNode(node: Node)
- :getNode() -> Node
- :onEnter()
- :pre_update(dt: number)
- :main_update(dt: number)
- :after_update(dt: number)
- :onDestroy()
- :cast(type: string) -> T



## RendComponentBase

- alpha
  - :getAlpha() -> number
  - :setAlpha(a: number)

- pipeline_config
  - :setBunchId(id: integer)
  - :getBunchId() -> integer
  - :setSideId(id: integer)
  - :getSideId() -> integer
  - .ConfigType
    - Inherit
    - Custom
  - :setConfigType(type: ConfigType(integer))
  - :getConfigType() -> ConfigType(integer)

- camera
  - :setCamera(cam: Camera)
  - :getCmaera() -> Camera

- color
  - :getColor() -> Vec3
  - :getColorR() -> number
  - :getColorG() -> number
  - :getColorB() -> number
  - :setColor(color: Vec3)
  - :setColorR(r: number)
  - :setColorG(g: number)
  - :setColorB(b: number)

- line_mode
  - :setLineMode(yesno: boolean)
  - :getLineMode() -> boolean

- multi_alpha
  - :getAlpha(p: integer) -> number
  - :setAlpha(p: integer, alpha: number)

- shader
  - :setShader(shader: Shader)
  - :getShader() -> Shader



## RendLineComponent: Component

- create(from: Vec3, to: Vec3, wide: number, node: Node) -> RendLineComponent
- :getPoint(p: integer) -> Vec3
- :setPoint(p: integer, v: Vec3)
- :getWide() -> number
- :setWide(wide: number)



## RendRectangleComponent: Component

- create(size: Vec3, node: Node) -> RendRectangleComponent
- :getSize() -> Vec3
- :setSize(size: Vec3)



## RendSpriteComponent: Component

- create(man: ResourceManager, path: string, node: Node) -> RendSpriteComponent
- create(man: ResourceManager, path: string, node: Node, wraps: string, wrapt: string) -> RendSpriteComponent
- create(sp: SpriteBase, node: Node) -> RendSpriteComponent
- :changeSprite(sp: SpriteBase)
- :getSpriteWidth() -> number
- :getSpriteHeight() -> number



## RendCircleComponent: Component

- create(radius: number, node: Node) -> RendCircleComponent
- :getRadius() -> number
- :setRadius(radius: number)
- :getPartition() -> integer
- :setPartition(part: integer)
- :getAutoPartition() -> boolean
- :setAutoPartition(yesno: boolean)
- :updatePartition()



## RendTextComponent: Component

- create() -> RendTextComponent
- :setText(text: string)
- :getText() -> string
- :setArea(area: Vec3)
- :getArea() -> Vec3
- :setFontSize(size: integer)
- :getFontSize() -> integer
- :setLineSpacing(spacing: number)
- :getLineSpacing() -> number
- :setAreaMargin(side: TextAreaMargin(integer), val: number)
- :getAreaMargin(side: TextAreaMargin(integer)) -> number
- :setTextAlignHorizontal(align: TextAlignHorizontal(integer))
- :getTextAlignHorizontal() -> TextAlignHorizontal(integer)



## PhysicsWorldComponent: Component

- create(node: Node) -> PhysicsWorldComponent
- :getWorldId() -> integer
- :setPhysicsStepCount(count: integer)
- :getGravity() -> Vec3
- :setGravity(g: Vec3)
- :getEnableContactCallback() -> boolean
- :setEnableContactCallback(yesno: boolean)
- :worldExplode(pos: Vec3, radius: number, impulse: number)



## PhysicsShapeBase

- :setFriction(friction: number)
- :setRestitution(restitution: number)
- :setDensity(density: number)
- :setFilter(categoryBits: integer, maskBits: integer, groupIndex: integer)
- // TODO



## PhysicsShapeBox: PhysicsShapeBase

- create(size: Vec3) -> PhysicsShapeBox



## PhysicsShapeCircle: PhysicsShapeBase

- create(radius: number) -> PhysicsShapeCircle



## AudioClip

- new(path: string) -> AudioClip
- new(mem: string(binary), length: integer) -> AudioClip
- new(man: ResourceManager, path: string) -> AudioClip
- :release()



## AudioPlayer

- new(audio: AudioClip) -> AudioPlayer
- :changeClip(audio: AudioClip)
- :play()
- :stop()
- :pause()
- :resume()
- :isPlaying() -> boolean
- :setVolume(to: number)
- :getVolume() -> number
- :getPos() -> integer
- :getFinPos() -> integer
- :setPos(to: integer)
- :setLoop(cfg: integer)
- :getSampleRate() -> integer



## Clipboard

- setClipboardText(text: string)
- getClipboardText() -> string
