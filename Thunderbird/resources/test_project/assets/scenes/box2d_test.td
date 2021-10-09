Scene Name: Untitled
Entities:
  - EntityID: 7314408732516647343
    Tag Component:
      Tag: Camera
    Transform Component:
      Translation: [0, 0.00295895338, 4.84377432]
      Scale: [1, 1, 1]
      Rotation: [0, -0, 0]
    Camera Component:
      Camera:
        ProjectionType: 1
        OrthographicSize: 1
        OrthographicNear: 1
        OrthographicFar: -1
        PerspectiveVerticalFOV: 0.785398185
        PerspectiveNear: 0.00100000005
        PerspectiveFar: 10000
      Primary: true
      KeepAspectRatio: false
  - EntityID: 4285495841213220498
    Tag Component:
      Tag: Square
    Transform Component:
      Translation: [0.460990727, 3.3310349, 2.98023224e-08]
      Scale: [0.999994516, 0.999994516, 1]
      Rotation: [0, -0, -2.34374213]
    Sprite Renderer Component:
      Color: [0.049167525, 0.528088212, 0.580645204, 1]
    Rigidbody2D Component:
      Type: 2
      Fixed Rotation: false
    BoxCollider2D Component:
      Offset: [0, 0]
      Scale: [1, 1]
      Density: 1
      Friction: 0.5
      Restitution: 0.100000001
      RestitutionThreshold: 0.5
  - EntityID: 2069409354346445702
    Tag Component:
      Tag: Ground
    Transform Component:
      Translation: [-2.98023224e-08, -1.39736247, 0]
      Scale: [10.9304457, 1, 1]
      Rotation: [0, -0, 0]
    Sprite Renderer Component:
      Color: [1, 1, 1, 1]
    Rigidbody2D Component:
      Type: 0
      Fixed Rotation: false
    BoxCollider2D Component:
      Offset: [0, 0]
      Scale: [1, 1]
      Density: 1
      Friction: 0.5
      Restitution: 0.100000001
      RestitutionThreshold: 0.5