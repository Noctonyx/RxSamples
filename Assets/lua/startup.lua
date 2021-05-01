dofile("/models/testcube.lua");

data.prototypes["testcube"] = {
    mesh = {"mesh/testcube", 0}
}

data.shaders["shader/imgui_vert2"] = {
    shader = "/shaders/shdr_imgui_vert.spv",
    stage = "vert"
};

