data.meshes["mesh/testcube"] = {
    mesh = "/models/testcube.mesh",
    vertices = 96,
    indices = 132,
    submeshes = {
      {
        first_index = 0,
        index_count = 132,
        material = 0
      },
    },
    materials = {
        "material/TestCubeMaterial",
    }
};

data.materials["material/TestCubeMaterial"] = {
    color_texture = "texture/Material Base Color",
    roughness = 0.548515,
    metallic = 0,
    alpha_mode = "OPAQUE"
};

data.textures["texture/Material Base Color"] = {
    image = "/models/Material Base Color.png",
    sampler = {
      minFilter = "linear",
      magFilter = "linear",
      addressU = "repeat",
      addressV = "repeat"
    }
};
