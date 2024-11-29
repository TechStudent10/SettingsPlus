# Converts the stuff I put in about.md into the macro for the mod

text_input = """- gv_0027 Auto Checkpoints
- gv_0067 High Start Position Accuracy
- gv_0068 Quick Checkpoint Mode
- gv_0071 Hide Practice Button
- gv_0100 Practice Death Effect
- gv_0135 Hide Attempts in Practice"""

for line in text_input.split("\n"):
    line = line[2:] # Remove the -
    gv = line.split(" ")[0].replace("gv_", "") # Retrive Game Variable
    name = line.replace("gv_" + gv + " ", "") # Retrieve Name
    print(f"SETTING(\"{name}\", \"{gv}\")")
