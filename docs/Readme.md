Welcome to Mercury engine's Github page!

Each engine must have single page website (made inside github.com) that contains:


1. Short introduction to the engine and it’s context (university subject).
[Mercury engine](https://github.com/knela96/Mercury-Engine) is a videogames engine university project for UPC-CITM. This project pretends to create an
engine witch is able to load, import and play animations from fbx models that have been animated in an external animation software such as maya, blender 
or 3DsMax. 
This s a group project by [Eric Canela Sol](https://github.com/knela96) and [Jaume Avinyó Sedano](https://github.com/Jaumeavinyo). We are third year students
in the programming and videogame developement Bachalor Degree in UPC-CITM.

2. Team photo and quick explanation of the individual work of each member.



[Jaume Avinyó:](https://github.com/Jaumeavinyo) 
- fbx importer and Game object structure
- console + its functionalities
- Engine state info such as fps, modules configuration's variables, hardware detection and software versions for all libraries used
- GameObjects Hierarchy (a part)
- Inspector for gameobjects
- Basic shapes loading
- Animations (Mostly done in group)

[Eric Canela:](https://github.com/knela96)
- fbx importer and DDS teture format importer
- Drag and drop files to the engine
- GameObjects transforms
- GameObjects Hierarchy (most of it)
- most of the engine's Imgui menus
- Camera Component with modifiable fov and frustrum
- Scene resizable
- AABBs for gameobjects
- Safe and load in own format
- Animations (Mostly done in group)


3. Short explanation of the main core sub-systems.
- Camera:
- File System:
- Importer
  - Material
  - Mesh
- Renderer
- Resources
- Animator

4. Detailed explanation of last assignment sub-system, containing two animated gif that shows editor
and in-game functionality.
<img src="AnimationsDiagram.png" >

So, as it can be seen in the image avobe, the animator is the core of this animations system. It contains all the animations
and you can choose whitch you want t reproduce calling DoAnimation(index) with the position on the list of the animation, this 
function just sets as the current animation the animation that you are passing with the index.
As Animator is a component, it has a Update function called every frame, in this function we are doing our core actions and to work we need
to do two things: Increase the current time of the animation, wich are the frames and Calculate the CurrentAnimationPose,
to do this we enter into the currAnimation wich is type Animation and we take the keyframes information
at the current time. 
We do not have keyframes for every time in the timeline, so we will need to calculate interpolation poses between keyframes,
this will be Animator's work.

Every joint has a list of children, a transform matrix and a index, wich matches the order in wich the joint is stored in te joints list. This index is used
by vertexes in the renderer to know wich bone is afecting them and wich trasformations should they take

So the list of keyframes inside animation is a std::map, this can store a list of joints(bones) with their names and transformations stored in a class JointTransform, wich 
has position, scale and rotation.
The animated object is the one that is animated by the Animator, this has a RootJoint that contains the root joint. Every Joint has a 
transform and a list of its childrens so also the root joint has this list. This is very usefull to create the hierarchy system of bones. joints also
have a index wich is very usefull because as I said before, vertexes need to know wich bone is afecting them.

The importer fills the animation list and their frames ussing assimp "aiAnimation", bones are loaded with aiBone and the last thing you need to load is every vertex influences.




5. One minute video speed-up that shows the process of authoring scenes in your editor, especially
with your core technology from the last assignment (captured from the computer, not mobile).


6. Link to the code repository and last release to download.


7. License.