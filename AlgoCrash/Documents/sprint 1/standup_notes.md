# Sprint 1 – Stand-Up Meeting Notes

This document includes each team member's answers to the three Scrum questions for four days of stand-up meetings. It is written in a rough, informal format as instructed.

---

## 📅 Day 1 – Monday

**Abdulrahman Al Hattali**  
1. Looked at how to create a new Qt + C++ project.  
2. I’m going to set up the Qt project structure.  
3. Nothing blocking yet, just figuring out the best way to structure files.

**Devin Gupta**  
1. I installed Box2D and started reading the integration guide.  
2. I plan to link Box2D with our Qt setup and test a small sample.  
3. Might need help with CMake configuration for both libraries.

**Siddharth Dhumal**  
1. I went through Box2D tutorials.  
2. I’ll begin setting up the rendering part in Qt.  
3. I need to understand how to pass Box2D object data to Qt graphics.

**Jungbin Moon**  
1. Looked into Box2D’s body and shape classes.  
2. I’ll work on setting up the ground block.  
3. No blockers, but I want to double-check scaling ratios.

**Yen Yee Tan**  
1. Read about basic UI elements in Qt.  
2. I will try adding a button for running the animation.  
3. No blockers for now.

---

## 📅 Day 2 – Tuesday

**Abdulrahman Al Hattali**  
1. Qt project is building properly.  
2. I’ll add placeholder code for a Box2D block.  
3. Trying to match coordinate systems between Qt and Box2D.

**Devin Gupta**  
1. Box2D compiles but not linked yet.  
2. Will work on linking and testing with a simple Box2D body.  
3. Might need help setting library paths on macOS.

**Siddharth Dhumal**  
1. Started reading Qt’s paint system.  
2. I will connect Box2D object data to the renderer.  
3. Need to figure out frame update timing.

**Jungbin Moon**  
1. Wrote basic Box2D ground setup.  
2. I’ll try to make a block land and stop on it.  
3. None.

**Yen Yee Tan**  
1. Added a test button to the window.  
2. Will hook it up to start/stop logic soon.  
3. Just need to decide on signal-slot connection style.

---

## 📅 Day 3 – Wednesday

**Abdulrahman Al Hattali**  
1. Added one falling block into the world.  
2. I want to fine-tune gravity and bouncing.  
3. Rendering delay is a bit off — might ask Siddharth.

**Devin Gupta**  
1. Box2D integration is finally working!  
2. I’ll test more shapes today and see how many we can support.  
3. None for now.

**Siddharth Dhumal**  
1. Connected one Box2D object to Qt visuals.  
2. Want to display multiple objects at once.  
3. Slight lag between physics and paint, will sync timing better.

**Jungbin Moon**  
1. Ground works and is detecting collisions.  
2. I’ll try to test with multiple falling objects.  
3. I might slow it down a bit — it falls too fast.

**Yen Yee Tan**  
1. Hooked up button to start/stop the simulation.  
2. Next, I’ll try to pause mid-simulation.  
3. No blockers so far.

---

## 📅 Day 4 – Thursday

**Abdulrahman Al Hattali**  
1. Finalized block creation and bounce effect.  
2. Will clean up and comment the code.  
3. None.

**Devin Gupta**  
1. Tested with more blocks and added slight random spawn offset.  
2. I’ll wrap up by helping with documentation.  
3. No blockers now.

**Siddharth Dhumal**  
1. Made the render smoother and added a tooltip stub.  
2. Want to test tooltip activation after block collision.  
3. None.

**Jungbin Moon**  
1. Final ground test looks good.  
2. Will prepare a basic screenshot for our UI mockup.  
3. None.

**Yen Yee Tan**  
1. Pause button works now.  
2. I’ll make a second button to reset the world.  
3. None.

---

---

## 🚀 Sprint Review & Retrospective

### 🧩 Sprint Review (What We Got Done)

- Box2D was successfully integrated into the project.
- We managed to simulate falling blocks that bounce on the ground.
- UI buttons were added to start, stop, and pause the simulation.
- The simulation logic and visuals are connected, with visible animation.
- We created a tooltip placeholder to explain block collisions.

### 💬 Retrospective (Team Reflections)

- Communication was consistent, and everyone showed up for meetings.
- We split tasks well and everyone contributed to core features.
- A few delays came from setting up libraries and understanding Qt-Box2D connection.
- Some people were learning new tools (Box2D, Qt), which slowed things slightly.
- For Sprint 2, we want to:
  - Start a bit earlier
  - Test in smaller chunks more often
  - Possibly pair up for UI + physics syncing

Overall, we were happy with our progress and feel ready for more complex logic in Sprint 2.