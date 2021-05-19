dofile("/models/testcube.lua");

data.visibles["visible/testcube"] = {
    objects = {
        {
            mesh = "mesh/testcube",
            submesh_id = 0
        }
    }
}

data.prototypes["prototype/testcube"] = {
    visible = "visible/testcube",
    world_position = true,
    rotation = true
}

data.shaders["shader/imgui_vert2"] = {
    shader = "/shaders/shdr_imgui_vert.spv",
    stage = "vert"
};
