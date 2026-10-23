# PLEASE INSERT COIN — Proposal and Code Structure Handoff

## 1. Purpose, sources, and revision status

Use this document to continue planning and implement the approved CS0045 C++ OpenGL/GLUT midterm game in a new conversation. Preserve the decisions and distinctions below. The user selected the game concept and story direction; do not replace them with a different genre or reveal the mystery early. Before writing code, inspect the user's course instructions and project/toolchain setup, then implement in the effort order below. Keep the first playable version small and aligned with the course modules.

**Revision status:** Frozen pre-implementation specification. The four instructional PDFs have been reviewed; no source code was built, no game was run, and no gameplay test has been executed or marked passed for this revision. The next development step is M0. Future changes should follow actual development findings, course requirements, or playtesting. Acceptance criteria remain future verification gates, not evidence of success.

**Source and precedence:** The original uploaded handoff supplies the confirmed game decisions, proposal, six effort phases, toolchain guidance, and working defaults. The general CS0045 project requirements quoted by the user in “Project Plan Review” supply the assignment requirements below; the four supplied Module 01–04 instructional PDFs have now been reviewed for the module references in Section 4. Those instructional PDFs are not the full project rubric. The actual VS Code workspace, installed toolchain, successful compilation, and game execution remain unverified pending M0. Earlier assistant replies were exposed only as content-reference placeholders, so the exact previously proposed heading names could not be recovered. This revision retains all 15 sections, all seven deliverables, and original tests T01–T30, and integrates the subsequent UI, writing, and mystery decisions throughout. The exact attachment PLEASE_INSERT_COIN_code_handoff(1).md was recovered from the referenced conversation; the current user request supplies the controlling decisions where assistant messages remain placeholders. Section 8 now includes the interface and machine-clue specification. Continuation instructions are consolidated into the build section. Visual decisions come from the user’s supplied description; no independent analysis of the named inspiration is claimed.

**Reading labels:** “Official project requirement” means a requirement in the general assignment instructions previously supplied by the user, not a requirement inferred from a module example. “Course instructional material” means topics/examples in the four reviewed Module 01–04 PDFs. “Project-specific decision” means an approved game choice; implementation suggestions/defaults remain adjustable. “Confirmed decision” means a decision labeled confirmed in the original handoff or in the user’s subsequent visual-design instructions. “Implementation recommendation” and “working default” are adjustable design choices, not extra course requirements. Where an older broad description admits several behaviors, the explicitly labeled prototype rules in Sections 6–7 make it implementable without upgrading a suggestion into a confirmed decision.

## 2. Short proposal

### Project Title

PLEASE INSERT COIN

### Problem/Idea – What are you creating?

An interactive C++ OpenGL/GLUT mini-game about an unreliable vending machine. A routine 2D snack purchase suddenly turns into an over-the-top 3D rescue mission: the player operates a maintenance claw inside the machine to retrieve the jammed snack. Three different customers make increasingly odd requests. Their connection is a gradual mystery: they are alien shapeshifters disguised as humans, but the game never states this directly. It ends on a visual clue.

### Purpose – What does the graphical application demonstrate or solve?

The application entertains the player while demonstrating course concepts through an interactive graphical program: functions, variables, conditions, loops, geometric primitives, coordinates and vertices, colors, GLUT display and input callbacks, text, arrays, and vertex arrays. It also demonstrates switching between 2D orthographic and 3D perspective scenes.

### Target User – Who would use or view it?

Casual players who enjoy short, absurd mini-games, and students or instructors viewing an interactive computer-graphics midterm project.

### Objects – What graphical objects will appear?

The scenes contain a vending-machine cabinet and glass, shelves, repeated snack packages, coin and selection buttons, customer portraits or simple figures, a maintenance claw and cable, the target snack, a moving restocking arm, the delivery chute, interface text, and the final reflection. Draw objects from simple points, lines, triangles, quads, and polygons. Avoid external models and imported textures in the first version. Reusable geometric buildings, windows, pavement, and streetlamps frame the glowing machine; oversized industrial details frame the interior. A later framebuffer color texture is a rendering target, not an external art asset.

### User Interaction – What can the user control?

The mouse inserts a coin, selects the requested snack, and collects a delivered snack. Keyboard controls move the claw across the machine's width and depth, lower it to attempt a grab, release a carried snack, pause or resume, and retry a stage. Claw movement is predictable and stops immediately; grab alignment is forgiving within a small tolerance; carried snacks do not slip. Stages use limited attempts and have no countdown.

### Expected Output – What should happen when the application runs?

The game opens on a cozy, quiet nighttime street in a retro arcade/Metroidvania-inspired pixel style, with the glowing vending machine dominating the 2D view. A customer requests a snack; after the player inserts a coin and selects it, the snack jams and the view makes a sudden, dramatic transition into a playable, surreal low-poly 3D industrial interior with a fixed, moderately elevated oblique camera and visible claw-position marker. The player moves the claw, retrieves the target snack, and drops it in the chute. Success returns to 2D, where the customer collects it and the next stage begins. Three stages increase the obstacle challenge and customer oddness. After the last customer leaves, their reflection remains in the vending-machine glass, turns toward the player, then disappears during a light flicker. No text explains the mystery.

## 3. Confirmed decisions, working defaults, and boundaries

### Confirmed by the user

- Title: **PLEASE INSERT COIN**.
- Genre: mini-game; the core idea is vending-machine snack rescue.
- A routine 2D scene should switch unexpectedly and dramatically into a 3D scene, then return to 2D.
- Core 3D gameplay: control the maintenance claw to rescue the jammed snack.
- Claw feel: precise and slightly forgiving. Movement should not drift; snacks should not randomly slip.
- Difficulty uses limited attempts and no countdown.
- Each of three stages follows a different customer with increasingly ridiculous snack requests.
- The customers are alien shapeshifters disguised as humans. Their identity is not revealed outright; clues accumulate gradually.
- Final clue is visual only: the last customer's reflection stays behind, turns toward the player, and disappears with a light flicker.
- The deliverable at planning end includes the seven-section short proposal above.

### Subsequently confirmed UI and mystery decisions

The interface belongs to the world: diegetic, atmospheric, mysterious, unexpected, and visually consistent with the cozy pixelated 2D street and surreal low-poly 3D interior. The quiet nighttime title uses **INSERT COIN** as its start action. Initial help teaches purchasing only; rescue controls appear contextually after the 3D reveal. Purchase information resembles a commercial vending display; rescue information resembles a restrained industrial maintenance terminal.

Pause shows the frozen, darkened scene with readable resume, restart/reinitialize, controls, and return-to-street options. Success uses **ITEM RECOVERED** and returns to the customer; failure uses **RECOVERY UNSUCCESSFUL** and offers retry. The ending has no victory banner or explanation and returns to **PLEASE INSERT COIN**.

The machine is **not confirmed sentient, alien, knowingly recognizing customers, or communicating with them**. Its tiny disturbances look involuntary and remain open to interpretation. Never implement customer profile matching, identity detection, personalized machine messages, or automated correct snack selection. The hidden customer premise does not establish anything about the machine's identity or intent.

The stage-specific disturbances and final three-light motif in Section 8 are confirmed artistic beats. Their exact timing, geometry, keybindings, internal fields/functions, menu mechanics, and all dialogue examples are proposed implementation defaults. Minimal requested menus are in scope; an elaborate menu system remains excluded.

### Working defaults, not final user decisions

- Three attempts per stage was suggested earlier, but the user only confirmed that attempts should be limited. Treat 3 as an easy-to-change prototype default, not a fixed requirement.
- Proposed snack progression: chips, chocolate bar, juice carton.
- Proposed layouts: open approach; tighter snack arrangement; moving restocking arm.
- Proposed keyboard bindings below are convenient defaults; keep them easy to change.
- Attempts are proposed to be spent on a failed grab or a drop outside the chute. A failed attempt should reset the snack and claw to known safe positions. Confirm tuning during playtesting.
- Customer dialogue and additional customer mannerisms below are editable examples; the specified machine-disturbance progression and final reflection beats are confirmed. Keep all clues subtle; do not add an explicit alien reveal or explanatory ending.

### Scope limits for the first playable submission

- One vending machine, one bounded playable 3D interior, three stages, three target snacks. Oversized scenery makes the interior feel unexpectedly enormous without expanding the navigable level or stage count.
- Reuse the same claw controls and scene geometry across stages; change target placement, obstacles, customer dialogue, and small visual clues.
- Use simple geometric customer portraits/figures, not detailed animated characters.
- Do not add physics simulation, inventory, random level generation, sound, external assets, elaborate cutscenes, or a complex menu unless the assignment or user later requires them.

### Strengthened narrative and scope boundaries

The alien-shapeshifter premise is internal design knowledge, not a reveal to deliver to the player. Do not introduce an alien model, transformation cutscene, explicit confession, explanatory final caption, or mandatory lore dialogue. Keep clues cumulative but deniable until the visual ending, and keep rescue gameplay readable. The final reflection sequence must retain all three beats: linger after departure, turn toward the player, disappear in a light flicker.

The 3D rescue is core confirmed scope, not an optional stretch goal. If rendering support is difficult, prove arrays in 2D first and diagnose the existing toolchain; do not replace the game with a 2D-only version or silently add a second compiler stack. Extra technical features should come from the planned movement, collisions, transitions, or moving obstacle, not an expanded genre.

No scoring economy, extra stages, alternate endings, procedural levels, real physics, asset pipeline, audio system, engine/framework, or elaborate menu is implied by the course checklist. Preserve one machine, three customers/stages, deterministic controls, limited attempts, and no countdown. Numeric tuning and sample dialogue remain editable defaults.

The confirmed visual direction and its adjustable rendering parameters are specified once in Section 8. Reusable geometric street and industrial decorations are within scope; they do not authorize additional mechanics, explorable streets, imported assets, or an engine.

## 4. Official CS0045 requirements

The following are **official project requirements from the general CS0045 instructions previously supplied by the user**. The course modules below teach relevant concepts but do not independently add project requirements. The module names and page references refer to the supplied PDFs; check the actual project rubric separately if its wording is needed.

| ID | Official requirement | Minimum or explanation |
|---|---|---|
| R01 | C++ with OpenGL/GLUT | Use C++ as the main language and OpenGL/GLUT as the graphics environment. |
| R02 | Course programming and graphics concepts | Meaningful functions, conditions, loops, variables, geometric primitives, coordinates/vertices, colors, GLUT callbacks, text, arrays, and vertex arrays. |
| R03 | Meaningful user interaction | At least five different meaningful actions; do not count a collection of color changes as sufficient. |
| R04 | Mouse and keyboard | At least one meaningful mouse interaction plus meaningful keyboard controls. |
| R05 | Distinguishable graphical objects | At least five visually distinguishable objects; a composite object should read as an object, not an isolated triangle counted to inflate the total. |
| R06 | Primitive variety | At least three OpenGL primitive types visibly used. |
| R07 | Application states | At least three application states, with decisions affecting the screen. |
| R08 | Arrays and vertex rendering | The general project assignment requires at least one appropriate vertex-array or indexed-vertex approach. Module 04 Subtopic 2 teaches `glDrawElements()` as course material; the handoff project recommendation demonstrates both draw paths. |
| R09 | Vertex-array explanation | Explain the stored data, reason for an array, meaning of each vertex, rendered primitive, and advantage over repeatedly specifying the same data. |
| R10 | Beyond-basics feature | At least one extra feature, such as animation, collision, menus, character movement, or interactive buttons. |
| R11 | Test cases | At least 10 project-specific cases covering normal operations, incorrect actions, boundaries, state changes, keyboard, mouse, and reset/restart. |
| R12 | Submission | All seven deliverables listed in Section 14. |



### Course Module References and Implementation Alignment

The supplied files are the primary instructional references for implementation technique:

| Source | Verified topics and pages in the supplied PDF | Project application and boundary |
|---|---|---|
| **Module 01.pdf** | Subtopic 1, OpenGL history/rendering pipeline: pipeline stages on pp. 8–13. Subtopic 2, simple C++/GLUT application: objectives p. 14; prescribed VS Code, Microsoft C/C++ extension, MSYS2 UCRT64 GCC, FreeGLUT, Windows OpenGL/GLU and no separate MinGW on p. 15; workspace layout p. 19; “Build OpenGL Program” task and active-file workflow p. 20; compile/run choices p. 21; simple `glutInit`, `glutCreateWindow`, `glutDisplayFunc`, `glutMainLoop` example pp. 24–28; vertices/primitive calls and window defaults pp. 30–31. | M0 inspects the real workspace/toolchain and records the actual build task/result; M1 initializes the window, registers display and draws. The pipeline overview includes programmable modern stages, but this project retains the taught legacy compatibility path unless the actual course setup requires otherwise. Reading p. 15 does not verify the local installation. |
| **Module 02.pdf** | Subtopic 1, Geometric Specification & Drawing: objectives p. 2; primitives/attributes and vertices p. 3–4; `glVertex2f`/vertex command format p. 5; points p. 6–7; `GL_LINES`, `GL_LINE_STRIP`, `GL_LINE_LOOP` pp. 8–12; optional line stippling pp. 13–17. Subtopic 2, Filled Primitives and Colors: objectives p. 18; triangles pp. 20–21; polygon pp. 22–23; triangle strip/fan pp. 24–27; quads pp. 28–31; color attributes/RGBA pp. 32–38. | M1/M2 draw machine panels and UI with quads, snack/low-poly faces with triangles or convex polygons, and shelf rails, claw cable and borders with lines. At least three types are visible in ordinary play. Shared geometric primitives and limited colors support the approved pixel-art and low-poly look. Line stippling is optional decoration, not a requirement. |
| **Module 03.pdf** | Subtopic 1, GLUT Extensions: raster positioning pp. 4–5; `glutBitmapString()` pp. 6–9; available bitmap and stroke font constants p. 10. Subtopic 2, GLUT Callbacks: objectives p. 11; window initialization/configuration pp. 13–17; `glutPostRedisplay()` p. 18; display and keyboard callbacks pp. 21–24; mouse callback pp. 25–26; idle and timer callback descriptions pp. 29–30. | M1/M2 use display rendering and redisplay after state changes; Module 3 keyboard and mouse callbacks drive menu/purchase/claw actions; bitmap text supports diegetic machine messages, dialogue, menu and HUD. Check the installed FreeGLUT headers before relying on `glutBitmapString()` or extension headers. Key-up, special-key, reshape and chosen timer/update behavior are FreeGLUT/project needs beyond the basic examples; the handoff's one bounded update mechanism remains a project-specific architecture choice. |
| **Module 04.pdf** | Subtopic 1, Vertex Arrays: purpose/overview pp. 2–5; immediate-mode `glBegin`/`glEnd` example p. 6; conversion examples pp. 7–9; `glVertexPointer()` size/type/stride/pointer parameters p. 10; `glDrawArrays()` mode/first/count p. 11; multiple-primitive example p. 12 and output p. 13. Subtopic 2, indexed rendering: `glDrawElements()` index-array example p. 14 and API/count/type/index-pointer specification p. 15; subtopic title/objectives p. 16; worked vertex/color arrays and client-array calls pp. 20–22. The p. 14 sample uses a `GLubyte` index array and `GL_UNSIGNED_BYTE`; the project must keep its own index storage type consistent with its draw-call type. Buffer objects are discussed conceptually on pp. 2–4. | M1 verifies a visible compatibility client-array draw on shelf rails or cabinet accents using `glEnableClientState`, `glVertexPointer`, and `glDrawArrays`. M5 remains the primary completion/documentation milestone: retain/refine the repeated `glDrawArrays` set and add the reusable indexed snack mesh using `glDrawElements`. The assignment's vertex-array minimum is tracked separately in R08; both draw paths are the project recommendation. Module 04's buffer-object discussion does not require VBOs, VAOs, shaders, or a new pipeline here. | 

The general project assignment previously supplied by the user is the source for R01–R12 in this handoff; it is distinct from the course instructional examples above. The modules are consulted for taught concepts and implementation context, not to promote every listed technique into a mandatory project feature. Project-specific decisions include the six-effort plan, M0–M6 checkpoints, early compatibility check, indexed snack mesh, fixed-step timing, render targets, and all gameplay/UI/narrative choices. M0 still has to verify the actual VS Code workspace, build task, compiler, libraries and visible compile/run result. PDF review alone verifies none of those local conditions.

**Implementation recommendation:** Demonstrate both `glDrawArrays()` and meaningful `glDrawElements()` rendering, exceeding the minimum wording “vertex array or indexed vertex approach” and covering the Module 4 indexed-rendering topic explicitly. The supplied assignment does not unambiguously say that both calls are separately mandatory. Three stages, 2D/3D switching, three prototype attempts, specific keys, the 51-case development checklist, and this handoff's 15-section layout are game decisions or planning recommendations, not additional official minimums.

## 5. Proposed game flow and stage content

### State flow

Use an explicit enum and state transitions rather than unrelated boolean flags. Minimum application states: **TITLE, CUSTOMER_INTRO, MACHINE_2D, TRANSITION_TO_3D, RESCUE_3D, TRANSITION_TO_2D, STAGE_RESULT, GAME_OVER, REFLECTION_OUTRO**. PAUSED may be represented as a saved prior state plus a pause flag, or as a PAUSED state with a resume state. This exceeds the course's minimum three states and makes the 2D/3D switch clear.

1. TITLE shows the quiet nighttime street and vending machine, the title PLEASE INSERT COIN, and the INSERT COIN start action. HOW TO PLAY opens purchase-only help; it does not preview rescue.
2. CUSTOMER_INTRO shows a brief request and the stage's customer portrait.
3. MACHINE_2D lets the player insert a coin and click the requested snack selection.
4. The selected snack jams. TRANSITION_TO_3D runs a short jam → warning → camera shift → rescue reveal transition, then enters RESCUE_3D.
5. In RESCUE_3D, the player positions the claw, lowers it to try to grab the target, carries it, and releases over the chute.
6. A successful delivery transitions back to the 2D `STAGE_RESULT` view; the player acknowledges collection once, a short customer reaction plays, and only then the next customer appears. Section 7 defines the result subphases.
7. At zero attempts, GAME_OVER offers retry of the current stage. Retrying resets that stage's attempts, claw, target, and moving obstacle.
8. After stage 3, the customer exits. REFLECTION_OUTRO plays the lingering reflection animation and ends on the ordinary vending-machine screen with “PLEASE INSERT COIN.”

### Three proposed stages

| Stage | Request/customer beat | Rescue layout | Gradual clue |
|---|---|---|---|
| 1 — Academic Emergency | An exhausted student asks for chips in ordinary language; one small qualification can sound slightly off. Use the editable examples below. | Chip bag with a clear approach; teach movement, lowering, carrying, and chute delivery. | Brief machine-display flicker and indicator disturbance near the customer; no alien traits. |
| 2 — Precision Nutrition | An office worker requests a chocolate bar with an unusually precise preference; any pronoun slip is optional editable writing, not a reveal. | Chocolate bar amid nearby packages; require deliberate positioning and a clear path. | Irregular selection-light pulses and a recurring subtle three-light pattern; no selection is made for the player. |
| 3 — Planetary Thirst | An overly cheerful visitor requests a juice carton; a casual plural or familiar remark sounds slightly misplaced but remains inconclusive. | Juice carton behind a moving restocking arm. | Maintenance indicator flickers before interaction; nearby machine lights dim as the customer approaches. Reserve the strongest reflection anomaly for the ending. |

Keep dialog brief so it supports the joke without interrupting play. Keep all clues in the 2D customer scenes or ending, so the 3D rescue remains legible. The final reflection is the strongest clue and must not explain what the customers are.

### Editable dialogue examples — not locked script

Keep the three customers conversational, brief, and gradually odd. Let phrasing and repetition carry the unease; avoid “human,” “species,” “Earth,” explicit alien jokes, technical exposition, or characters explaining the machine's behavior. Stage names remain internal proposed labels and need not appear on screen. Do not imply that the machine understands their requests.

| Customer | Editable request | Editable recovery reaction |
|---|---|---|
| 1 — tired student | “The chips, please. I've been here all evening.” | “Thanks. That's better.” |
| 2 — office worker | “The chocolate one. The wrapper needs to stay on.” | “Thanks. That's better. I'll leave it wrapped for now.” |
| 3 — cheerful visitor | “A juice carton, please. We weren't sure you'd still be here.” | “Thanks. That's better. You can go now.” |

These lines are proposed text, not additional lore. Adjust for natural delivery; the repeated reaction is a quiet option, not a required password or clue gate. Use one short line at a time in the bottom dialogue band, outside purchase controls. No branching dialogue, text-entry puzzle, mandatory reading timer, or machine reply is needed.

## 6. Precise gameplay invariants

These rules make the confirmed precise, forgiving claw and limited-attempt design concrete. Numeric tolerances, attempt charging, collision response, and action gating below are **implementation recommendations/working defaults** until tuned or approved; the no-drift, no-random-slip, no-countdown behavior remains confirmed.

| Area | Invariant / proposed prototype behavior |
|---|---|
| State ownership | Exactly one top-level `GameState` is active. A pause overlay stores the resume state. Rendering reads state; only update/input helpers change gameplay state, attempts, or stage progress. |
| Stage range | `stageIndex` stays in `[0, 2]`. Complete stage 3 by entering the outro, never by indexing stage 4. Completing one stage advances exactly once. |
| Attempts | `startingAttempts` is configurable, initially 3. Maintain `0 <= attemptsRemaining <= startingAttempts`. One resolved failed grab or invalid drop costs exactly one attempt. Starting a descent, holding a key, blocked motion, wrong purchase selection, pausing, and successful delivery cost none. |
| Attempt resolution | A grab/drop cycle has one outcome: successful carry, successful delivery, or one failure. Route all failures through one resolution helper with an action/cycle guard; repeated timer ticks must not deduct again. On zero remaining attempts, enter `GAME_OVER` immediately after resolution and reject rescue input. |
| Purchase | A stage begins with no coin and no accepted selection. One coin enables selection. Extra coin clicks are ignored with optional feedback. Selection before a coin is ignored. A wrong selection leaves the coin credit available and costs no rescue attempt. A correct selection consumes that credit and starts the jam/transition once. |
| Claw substates | Recommended precise substates: `IDLE_EMPTY`, `DESCENDING`, `ASCENDING_EMPTY`, `ASCENDING_WITH_SNACK`, `CARRYING`, `RESOLVING_DROP`. An empty Space press starts one descent; acquisition attaches one target and lifts it before entering `CARRYING`. Failed descent resolves once, then resets. A release resolves before another action is allowed. |
| Motion | X is width, Z is depth, Y is vertical. Horizontal input is allowed only in stable `IDLE_EMPTY` or `CARRYING`. Descent/lift is vertical and automatic. Reject horizontal motion during descent, ascent, or drop resolution. No inertia; releasing a key stops further commanded motion. Opposing inputs cancel; cap diagonal speed to the straight-line speed. |
| Cabinet bounds | Clamp/check the claw's physical footprint, not only its center. When carrying, include the snack's extents. Neither may cross the compartment or pass through shelves/packages. All start/reset poses must be clear of obstacles. |
| Grab test | Only the current target is grabbable. At the grab height, require `abs(clawX-targetX) <= grabToleranceX` and `abs(clawZ-targetZ) <= grabToleranceZ`, plus a clear descent/reach path. Equality is accepted. Use named nonnegative tolerances; tune them against visible geometry rather than inventing fixed world-unit numbers before scale is chosen. |
| Carrying | At most one snack is attached. Its position follows the claw with a fixed offset; it never randomly slips. Non-target packages remain obstacles/scenery. Clear the attachment before reset, stage change, or game over. |
| Chute test | Resolve release using the snack's release X/Z and the chute's documented rectangular acceptance region. Prototype recommendation: accept a snack center on or inside the region, with a visible guide matching that rule. Animation after release cannot change the recorded outcome. This specifies the original broad “overlaps chute bounds” wording; full-footprint containment is an optional later tuning choice, not a hidden rule. |
| Failed attempt reset | Restore target, claw, claw substate, and obstacle phase to the current stage's safe starting layout; clear held keys and active action flags. Preserve the decremented attempts and stage identity; keep the purchase completed so a remaining attempt resumes directly in `RESCUE_3D`. |
| Retry reset | Restore the same stage's full configured attempt count, target, claw, obstacle phase, held keys, pause state, and action flags. Proposed retry destination is `RESCUE_3D`, without repeating the purchase or advancing the customer. This differs from a full new-game reset. |
| Successful delivery | Record delivery once, remove the carried target, and begin `TRANSITION_TO_2D`. Preserve remaining attempts until the next stage initializes. Collection/acknowledgment advances the customer once; never award success twice from overlapping callbacks. |
| Pause | Pause is allowed during active rescue, including a descent/lift. Freeze claw, carried snack, moving arm, and action animation; preserve positions and progress. Clear held movement keys on entry and resume. Resume needs a fresh key press and must not apply elapsed paused time as a movement jump. |
| Timing | Use one registered update mechanism and elapsed time from `glutGet(GLUT_ELAPSED_TIME)` (or the course-supported equivalent). Clamp large frame deltas; preferably run gameplay in a small fixed step with bounded catch-up while rendering remains independent. Keep gameplay time separate from presentation time. No countdown or time-pressure failure exists; an internal elapsed clock only advances motion/animations. Pause freezes rescue movement, claw actions and obstacle simulation, and resume resets the time baseline. |

### Small data model and state ownership — recommendations

Keep the explicit top-level `GameState` and rescue `ClawState` as the authoritative state machine. Use a small set of ordinary C++ structs and enums, not mandatory class boundaries. A compact option is `GameData` (current state/stage, attempts, purchase/result flags), `ClawData` (position, held keys, claw substate, carried target), three fixed `StageData` records (target/layout/dialogue/clue cue definitions), `AnimationData` (transition, cue and outro phases/times), and `MenuData` (overlay owner, selection, confirmation). Merge or omit fields when an existing value already represents the same fact. Do not add redundant booleans that can disagree with `GameState`/`ClawState`, or a general purpose event history.

Input callbacks validate state, key/button edge, viewport and hit target, then dispatch a small action to gameplay or menu helpers. Those helpers alone mutate authoritative state. Route all completed failed grabs/drops through one `resolveFailure(reason)` helper, guarded so one action cycle deducts at most once; it owns exhaustion transition. `initializeNewGame`, `initializeStage`, `retryStage`, and `returnToTitle` have distinct responsibilities: full reset; next-stage setup; current-stage retry with attempts restored; and clearing run state to unpaid title. A failed attempt reset preserves remaining attempts and completed purchase. Menu navigation changes only menu data; confirmation invokes the relevant reset/transition helper. Cue and ambient updates can change presentation data only, never purchase, claw, attempts, or progression.

### GLUT timing contract — proposed implementation default

Register one timer/update chain. On each callback, read elapsed milliseconds, compute and clamp delta (suggested maximum 50 ms), accumulate gameplay time, and run fixed simulation steps (suggested 10 ms) up to a bounded catch-up count; discard excess backlog after a long stall rather than teleporting the claw. Update claw movement, descent/ascent, collisions and moving arm from fixed simulation steps. Use the same central update to advance non-gameplay presentation clocks with a clamped delta when their states are active. `display` renders the current snapshot only; it never advances gameplay or animation phases. Request redisplay after updates and reschedule exactly one next timer callback.

Gameplay clock runs only in active rescue. Pause and its nested Controls/confirmation overlays freeze the scene pose, obstacle and claw/action phases; reset the elapsed baseline when resuming. Title ambience may use presentation time. Customer cues begin from the authored customer-arrival event and use deterministic elapsed time; the outro advances ordered phases once even if a callback crosses a boundary. State changes initialize their own phase/time once; duplicate callbacks cannot restart a cue, transition, result, or outro. Internal animation time is not a countdown, does not appear as remaining time, and never causes failure.

### Obstacles and reachability

Use simple axis-aligned boxes/rectangles appropriate to the chosen movement height. Test the proposed movement segment or bounded small substeps so fast motion cannot tunnel through an obstacle. For the moving arm, reject an update that would overlap the claw or carried snack; do not teleport it through the player or arbitrarily remove attempts. The player must still have a way to move away. This conservative collision response is a prototype recommendation, not a physics system.

Provide at least one playable route from the start to the target and then to the chute in each stage. Stage 3 may require waiting for the arm to clear, but cannot create a permanent trap. Test the carried snack's larger footprint and the full arm sweep. If a layout cannot satisfy this, change the layout rather than weakening the confirmed reliable controls.

## 7. State-aware input specification

Bindings and exact state gates are **working defaults**. Keep on-screen instructions synchronized with the implemented controls. Mouse actions are left-button-down events; one click must produce one action. No hidden controls should bypass a stage or reveal the mystery.

| State / condition | Accepted input | Result / other input handling |
|---|---|---|
| `TITLE` | Up/Down selects INSERT COIN or HOW TO PLAY; Enter/click activates (default INSERT COIN) | Start initializes stage 1 and shows `CUSTOMER_INTRO`; title action does not grant purchase credit. HOW TO PLAY opens purchase-only help and closes back to title. Gameplay keys/clicks do nothing. |
| `CUSTOMER_INTRO` | Enter or click Continue | Enter the unpaid `MACHINE_2D` purchase view. Do not allow the same event to also buy a snack. |
| `MACHINE_2D`, unpaid | Click coin slot | Add one coin credit. Snack clicks show “Insert coin first”; rescue keys do nothing. |
| `MACHINE_2D`, paid | Click a snack button | Correct target starts the jam and `TRANSITION_TO_3D`; wrong target shows brief feedback without spending an attempt or the coin credit. Further coin clicks do not stack credit. |
| `TRANSITION_TO_3D` / `TRANSITION_TO_2D` | No gameplay input | Timer advances the transition once. Purchase, movement, Space, pause, and retry inputs are ignored. Clear held keys at transition boundaries. |
| `RESCUE_3D`, stable empty/carrying | Hold A/D or Left/Right; W/S or Up/Down | X decreases/increases; W/Up moves deeper into the machine (define this as increasing Z), S/Down moves toward the front (decreasing Z). Camera and labels must agree with this mapping. |
| `RESCUE_3D`, `IDLE_EMPTY` | Space key-down edge | Begin one grab cycle. Holding Space or key repeat cannot start additional cycles. |
| `RESCUE_3D`, `CARRYING` | Space key-down edge | Release once and resolve chute success or failure. |
| `RESCUE_3D`, descent/ascent/resolution | P or Esc | Pause; motion and action keys are otherwise ignored until stable. |
| `RESCUE_3D`, any substate | P or Esc | Pause and save this exact rescue substate. R is ignored during active play by default; no free refill of attempts mid-action. |
| Paused rescue, main menu | P/Esc resumes; Up/Down selects; Enter or click activates the focused option | Resume exact state; REINITIALIZE (Restart stage) opens confirmation; CONTROLS opens rescue help; RETURN TO STREET opens confirmation. Direct R, Space, purchase, and motion input remain blocked; key releases clear flags. |
| Help or confirmation overlay | Up/Down where choices exist; Enter/click; Esc goes back | Overlay consumes input exclusively. Help returns to its owner. Confirmation defaults to Cancel; Esc cancels. P does not bypass a child overlay. |
| `STAGE_RESULT`, awaiting collection | Click delivered snack/chute area; Enter as equivalent acknowledgment | Collect once, hide collection control, show customer reaction. A subsequent fresh Enter/click Continue completes the reaction and loads next `CUSTOMER_INTRO`, or enters `REFLECTION_OUTRO` after stage 3. No auto-advance; motion/purchase/pause/retry are ignored. |
| `GAME_OVER` | R; Enter when Retry is focused; or click Retry | Each activation calls the same authoritative `retryStage()` helper, restores the current stage, and returns to rescue without advancing progression. Use key-down edges/repeat suppression so holding R or Enter cannot retry repeatedly. |
| `REFLECTION_OUTRO` | No gameplay input | Play the visual clue in order, then return to ordinary title/machine screen with “PLEASE INSERT COIN.” A later INSERT COIN action begins a new game. |
| Any state | Window close | Close through the platform/FreeGLUT lifecycle. Esc means pause where allowed, not an undocumented exit. |

**Collection decision:** Retain explicit collection as the selected implementation default, not a newly confirmed artistic decision. `STAGE_RESULT` has `AWAIT_COLLECTION` and `REACTION` subphases: one acknowledgment collects; a second fresh acknowledgment continues after the reaction is visible. There is no automatic stage advance or minimum timed reading gate. Input cannot acknowledge two phases or the next customer in one event. Delivery is recorded once before entering this state; stage progression is committed once after the reaction.

### Minimal menu ownership and reset semantics — proposed defaults

Keep one top-level state with a small overlay mode (`NONE`, `PAUSE_MENU`, `HELP`, `CONFIRM_RESTART`, `CONFIRM_STREET`) and its owner. Pause is still **rescue-only**, including descent, ascent, and resolution before its outcome commits. All title/intro/purchase/transition/result/failure/outro states reject P/Esc as pause commands. Title help is a separate overlay, not a paused game. No extra global escape menu is required.

| Menu action | Proposed behavior |
|---|---|
| RESUME | Close pause; preserve exact rescue substate and animation progress; clear held keys and restart the update baseline. |
| REINITIALIZE (Restart stage) | Explain that current rescue progress is discarded and attempts reset. On explicit confirmation call the same clean stage reset as game-over retry, returning to current-stage `RESCUE_3D`; preserve completed purchase and customer identity. This is an intentional menu restart, not an active-play R shortcut. |
| CONTROLS | Keep rescue frozen while showing current rescue bindings; Back/Esc returns to the same pause selection without resuming. |
| RETURN TO STREET (End current run) | Explain that current run progress will be discarded. On confirmation clear run, coin, attachment, action, overlay, clue and outro state and return to the quiet `TITLE` street. INSERT COIN starts stage 1 anew; there is no suspended save or street exploration. |

Exact parenthetical labels and confirmation text are editable, but their functional meaning must remain visible. Proposed keyboard focus defaults to Resume; arrow navigation wraps through enabled options, mouse hover/click uses the same visible rectangles, and focus uses an outline plus brightness rather than color alone. Enter or one left-button-down activates only one option. Confirm dialogs default to Cancel and state the consequence; no destructive menu action occurs on merely opening it. Help exposes only purchase before the reveal and rescue controls from paused rescue.

Input dispatch order is window lifecycle, key-release cleanup, active overlay, then active-state actions. Consume each event once, suppress held/repeated activation, and require release before another action after any state/overlay boundary. Reset the animation time baseline after frozen overlays; no elapsed pause time is applied to rescue. Confirmation/help nested inside pause must not create additional timer chains.

### Input callback ownership

Callbacks are adapters: normalize/edge-detect the physical input, validate the active `GameState`, overlay owner and content viewport, then call one named action such as `insertCoin`, `selectSnack`, `moveClaw`, `requestGrabOrRelease`, `acknowledgeResult`, `handleMenuInput`, `pauseRescue`, or `resolveFailure`. Keep state-changing rules in those helpers, not duplicated in normal-key, special-key and mouse callbacks. State transitions go through a small helper that sets the new state and initializes its associated substate/time exactly once. Keep independent physical held-key flags so releasing one equivalent binding does not cancel another still held; clear them at reset, pause and state boundaries.

### Input implementation details

Register normal and special key-down/key-up callbacks using the installed FreeGLUT headers. Normalize letter case, track each physical binding so releasing A does not cancel a still-held Left arrow, and clear flags on pause, reset, and state changes. Use edge-triggered actions or key-repeat suppression for Space, pause, retry, and acknowledgment. Handle loss of focus/visibility where the available FreeGLUT callbacks permit, and check for stuck movement on return.

Use the shared logical 2D coordinate system with bottom-left origin for orthographic drawing. Convert GLUT top-left mouse Y using the current window height, then map through the displayed content viewport to logical coordinates; reject letterbox bars and any point outside the active content rectangle before hit testing. Use the same named layout rectangles for drawing and clicks. Guard zero-size resize values. Window resizing changes viewport/presentation mapping only, never gameplay coordinates, speed, tolerances, collisions or chute rules. Handle focus/visibility loss where supported and clear held keys on return.

### Meaningful interaction checklist

The original checklist is retained below. Count movement directions as one category; also demonstrate purchase, claw action, pause, and recovery rather than counting every key as a separate interaction.

Ensure at least five different meaningful actions are implemented and shown in the instructions:

1. Insert a coin with the mouse.
2. Select the requested snack with the mouse.
3. Move the claw left/right and forward/backward with keys.
4. Lower the claw and attempt to grab with Space.
5. Release a carried snack with Space and deliver it into the chute.
6. Pause/resume.
7. Retry the current stage after failure.
8. Collect the delivered snack, then acknowledge the customer reaction under the selected Section 7 default.

Movement directions are one category of action; make sure purchase, grab/release, pause, and retry also exist so interactions are not merely movement or color changes.

## 8. Visual Design and Rendering Specification

### Confirmed visual direction

These are **confirmed user decisions**, separate from the official CS0045 requirements. The named inspirations describe visual mood and form only; they do not add Metroidvania traversal, voxel simulation, copied assets, or new game mechanics.

| Aspect | Confirmed specification |
|---|---|
| 2D style and setting | Retro arcade and Metroidvania-inspired pixel aesthetics; a cozy, quiet nighttime street with the glowing vending machine as the primary focus. |
| 2D composition | Machine-dominant framing; customers stand beside the machine; dialogue sits near the bottom, outside the machine’s interactive controls. |
| 3D style and scale | Surreal, atmospheric low-poly graphics inspired by *Lucid Blocks*: chunky geometry, visible pixelation, and an exaggerated industrial vending-machine interior that appears unexpectedly enormous. |
| Camera and navigation | Fixed, moderately elevated oblique perspective during rescue, with a visible claw-position marker. Keep target, claw, safe routes, moving arm, and illuminated chute readable. |
| Shared presentation | Consistent retro pixelation across both world views, moderate detail built from reusable geometric decorations, and a shared palette shifting from warm amber/cream light in 2D to cool blue-green surroundings with isolated warm highlights in 3D. |
| Contrast and narrative | A dramatic contrast between the ordinary cozy exterior and strange interior. Preserve the three customers, subtle mystery, and purely visual lingering-reflection ending. |

### 2D environment and composition

Build the street from a small reusable set of geometric buildings, window patterns, pavement blocks, curbs, and streetlamps. Draw restrained background silhouettes first, pavement and lamps next, then the focal machine and the customer beside it. Use grid-aligned rectangles, stepped outlines, and limited-color shape clusters to suggest pixel art without external sprites. Reuse arrays/loops for windows and pavement rather than individually coding every decoration.

Use the machine’s amber/cream panels and glass highlights to establish the brightest focal area; nearby streetlamps and warm windows support it without competing. Keep the street quiet through restrained decoration and motion. Preserve space beside the machine for each simple geometric customer and a bottom dialogue band for short lines. The machine controls, coin slot, and collection area must remain visible and clickable. Keep glass readable enough for the final reflection to linger, turn, and disappear during the brief light flicker; ordinary reflections must not reveal the mystery early.

### 3D environment, camera, and visibility

Use oversized shelves, machinery blocks, supports, pipes, vents, bolts, and snack packages to imply an enormous industrial compartment. Construct moderate detail from reused cuboids, wedges, cylinders approximated with few sides, and line accents. Distinguish decorative distant machinery from gameplay collision boxes: scenery outside the bounded play area suggests scale but introduces no extra routes or unreachable objectives. Any decoration inside the playable route must either have matching collision or be placed where it cannot confuse navigation.

Keep the oblique camera moderately elevated and fixed throughout active rescue. Exact camera position, target, field of view, and clipping distances remain tuning parameters. Compose the full legal claw range, target, and chute within the useful view; avoid tall foreground geometry hiding them. The transition may move the camera before rescue begins. Do not introduce free-look controls or continuous camera shake during play.

Draw the claw-position marker beneath the claw at its actual X/Z, on the relevant play surface, with strong shape/value contrast and enough size to survive pixelation. It must not imply that a target behind an obstacle is reachable. Keep the target highlight distinct from the marker. Give the chute an illuminated rim or warm panel and a visible acceptance guide matching Section 6; decorative glow must not enlarge its gameplay acceptance region.

### Shared palette, lighting, and detail

Use shared named palette entries rather than unrelated colors per scene. Warm amber and cream dominate 2D light sources; cool blue-green surfaces dominate 3D, with warm accents on the chute, warning indicators, and selected machinery. Exact RGB values remain adjustable. Use silhouette, brightness, and shape as well as hue to distinguish interactive objects, obstacles, and scenery.

**Proposed first implementation:** simulate light with geometric light pools, bright solid panels, and simple face-color shading. Flat face colors suit chunky low-poly geometry and the legacy vertex-array path. Fixed-function lighting may be considered if supported by the existing setup, but is not required to achieve the look. If normals or face-local colors require duplicated vertices, document that choice in the indexed mesh. Avoid dynamic shadow systems, bloom pipelines, or shader dependencies.

Fog is optional and adjustable; apply it only to the 3D environment and keep the playable depth range clear enough to identify target, marker, obstacles, and chute. Disable or reduce any effect that obscures decisions. Restore fog, lighting, depth, blending, and texturing state before drawing 2D scenes or overlays. The mystery must come from staging and visual clues, not unreadable gameplay.

### Pixelation targets and staged implementation

The consistent pixelated appearance is confirmed. **Exact render dimensions, lighting implementation, and animation timings are adjustable development parameters**, not immutable decisions or official course requirements.

| Parameter | Proposed working default | Constraint |
|---|---|---|
| World render resolution | 320 × 180 | Target for both 2D and 3D; adjust if required for gameplay readability. |
| Window/presentation size | 1280 × 720 | Gives 4× integer enlargement of 320 × 180. Resizing must remain supported. |
| Alternate world resolution | 640 × 360 | Configurable alternative; gives 2× integer enlargement at 1280 × 720. A simple setting/constant is sufficient; no settings menu is required. |
| Enlargement | Nearest-neighbor filtering | Preserve hard pixel edges; do not use smoothing filters for the world image. |
| HUD and dialogue | Separate window-resolution overlay if needed | Legibility takes priority over forcing text into the low-resolution world buffer. Use a compatible bitmap font and consistent palette/layout. |

**First playable path:** render directly to the window using geometric pixel-style 2D shapes, chunky 3D geometry, face shading, and the shared palette. Implement the fixed camera and marker early. This establishes the visual direction while purchase, rescue, attempts, pause/retry, and three-stage flow are completed. Do not claim that direct window rendering provides the final uniform low-resolution pixel grid.

**Later enhancement:** after core gameplay is functional, inspect the available compatibility OpenGL framebuffer support. Render both world views into the same configurable low-resolution color target, with a depth attachment for 3D. Verify framebuffer completeness before use. Present the resulting texture with nearest-neighbor sampling, then draw the legible HUD/dialogue overlay in window coordinates. Check texture orientation, viewport/projection restoration, depth state, and client-array state across passes. Keep resource creation/resizing/destruction out of per-frame gameplay logic; avoid allocating targets every frame.

Prefer a 16:9 content rectangle and integer enlargement where it fits, centered with letterboxing for other aspect ratios. A proposed small-window fallback is aspect-preserving nearest-neighbor scaling when an integer scale cannot fit; it may produce uneven pixel widths, so record that limitation. Derive mouse mapping from the actual displayed content rectangle, not from the full window or assumed 1280 × 720 dimensions. Map world controls into logical 2D coordinates and overlay controls into their actual overlay rectangles; reject clicks in the bars. World resolution changes must not alter collision, tolerance, or claw speed.

Framebuffer support and entry-point availability must be checked in the installed course environment; FreeGLUT window creation alone does not establish that support. Reuse existing project facilities where present. Do not add an engine, shader framework, second compiler, or dependency solely for pixelation. If the enhancement is unavailable or incomplete, retain a working direct-render fallback, document the unfulfilled uniform-pixelation target, and continue validating core requirements. The enhancement is not a gate for the first playable build.

### Coordinate contracts, OpenGL state and readability

**Coordinates.** Use bottom-left origin for one logical 2D scene space (proposed 320 × 180 logical units); title, street, machine, dialogue and hit rectangles share it. Convert mouse input from GLUT's top-left window coordinates, then through the actual 16:9 content rectangle/letterbox to that logical space. World/overlay rendering may use separate projections, but overlay hit rectangles use their matching overlay space and the same presentation transform. The 3D machine world uses X=width, Y=height, Z=depth consistently. Name gameplay bounds, camera values, claw dimensions, tolerances and chute bounds. The 320 × 180 render target and configurable 640 × 360 alternative, with proposed 1280 × 720 window, are rendering parameters; collision and movement use world coordinates and do not depend on them.

**OpenGL pass ownership.** Keep small setup/restore helpers for 2D orthographic world, 3D perspective world, pixel-target presentation, and window-space HUD/overlay. Each pass sets the projection and model-view matrices, viewport, depth test/write, lighting, blending, texturing and client-array state it needs, then restores known state before the next pass. In particular, disable depth/lighting/texturing as needed for bitmap text and 2D overlays; restore depth/projection for 3D; bind/unbind optional framebuffer and texture explicitly; disable client arrays and clear unexpected buffer bindings around legacy client-memory pointers. Do not rely on state left by a previous scene. Keep compatibility OpenGL/FreeGLUT and existing course libraries; shaders, modern pipelines and dependencies are unnecessary unless the actual course configuration requires them.

**Gameplay readability.** At both internal resolutions, distinguish target snack, other packages, claw, floor marker, obstacles/traversable space and chute with silhouette, outline, value and text/shapes as well as hue. The chute's visible acceptance guide matches its actual collision rectangle. Keep title, request, HUD, pause/help and result text readable at intended window size and long enough to read; menu focus is visible by outline/brightness as well as color. Flicker is brief and restrained, with no rapid high-contrast strobe. Fog, darkness, dithering and pixelation never hide gameplay-critical information. The pause darkening preserves enough of the frozen rescue to show it is suspended. Stage clues may remain easy to overlook; do not amplify them into instructions. Keep the final reflection perceptible through its linger, turn and disappearance, without adding an explanation.

### Transition and ending

Use one short timer-driven sequence: visible snack jam, brief warning effect, camera/projection shift, then the industrial rescue reveal. Use an ordinary, brief jam notice such as “DISPENSING INTERRUPTED” (editable). The earlier “CRITICAL SNACK FAILURE” example is superseded by this restrained default. Do not show rescue instructions, a 3D preview, or explanatory maintenance text before the reveal; industrial text begins only when the interior is visible. Exact durations and optional small shake/flash amounts are tuning parameters; do not require an elaborate cutscene system. Ignore gameplay input while the transition runs and enable rescue only when the camera and marker are stable. Preserve the short return to the cozy 2D result view after successful delivery.

Stage 3 still ends with the customer leaving, the reflection remaining and turning toward the viewer, then disappearing in a light flicker. No explanatory text, explicit transformation, or new ending is added. Check the reflection at both proposed world resolutions so pixelation or lighting does not erase the intended visual clue.

### Diegetic interface and writing contract

The UI should feel printed on, lit within, or overlaid by the machine, using the shared palette, compact bitmap typography, thin panel borders, and sparse indicators. Legible window-resolution overlays remain allowed; diegetic presentation must not make controls difficult to read. Do not add decorative scanline noise, rapid flashing, full-screen warning spam, unreadable fonts, scores, lore logs, or a settings system. Reserve the mystery for small visual disturbances and the reflection.

| Surface | Required appearance and information | Confirmed text / editable copy boundary |
|---|---|---|
| Title and initial help | Quiet street, glowing machine, minimal panel-like actions. Explain coin insertion and requested-snack selection only. Starting does not buy the snack. | PLEASE INSERT COIN title; INSERT COIN start action. HOW TO PLAY and purchase-help sentences are editable labels. |
| Purchase HUD | Ordinary commercial display: request, credit status, selection and dispensing feedback. No attempts, claw schematic, rescue controls, customer identity, or ominous profile readout. | “Credit: 0/1”, “Select item”, “Please check your selection”, and “DISPENSING INTERRUPTED” are editable examples. Wrong choice preserves credit; the player must choose correctly. |
| Rescue HUD / contextual help | Restrained industrial maintenance terminal, clear requested target, attempts remaining, motion/action/pause controls; clear marker and chute. Reveal help on first entry after the camera is stable. | “MAINTENANCE ACCESS”, “Attempts: N”, “Space: lower claw” / “Space: release” are editable. During descent/ascent show status, not an enabled action prompt. No countdown. |
| Pause | Exact frozen rescue scene darkened beneath a readable maintenance-style panel. Its four functional options remain available by keyboard and mouse. | Resume, restart/reinitialize, controls, return to street are confirmed functions; exact thematic labels are proposed in Section 7. |
| Success | Quiet status in the return/result presentation; delivered snack and customer stay central. Show collection prompt, then reaction/Continue without overlapping hit targets. No score splash or triumphant banner. | ITEM RECOVERED is confirmed. “Collect item — Enter / click” and “Continue” are editable. |
| Failure | Freeze the failed rescue backdrop with a restrained bureaucratic maintenance message and a visible Retry action. No accusatory joke or lore explanation. | RECOVERY UNSUCCESSFUL is confirmed; “Retry — R / Enter” is editable; Enter activates the focused Retry option. |
| Ending | After last reaction, customer departure, lingering reflection, turn and disappearance are unobstructed by dialogue, HUD, prompts or result text. Restore ordinary title only after the flicker resolves. | No victory banner, “The End”, explanation, or machine message. Return to PLEASE INSERT COIN with INSERT COIN available for a fresh run. |

Contextual rescue teaching is a compact nonmodal prompt once the interior is revealed, alongside normal rescue HUD hints; do not require another tutorial acknowledgment before movement works. Show movement/lowering when empty, carrying/release guidance when attached, and attempt feedback after failure. Pause Controls repeats the same bindings. Exact wording is editable and must come from the same control definitions used by input handling.

### Deterministic machine disturbances — confirmed beats, proposed timing

Use a short authored cue timeline driven by scene-local elapsed time and one-shot flags, never randomness or customer recognition logic. The scene already knows its stage for layout/dialogue; this is staging, not a machine profiling subsystem. Render functions read cue intensities without consuming events. No clue changes credit, target, attempts, collision, selected item, input gates, success, or stage advancement. The player may miss every cue and still finish normally.

| Beat | Confirmed visual clue | Proposed trigger and timing, adjustable after observation |
|---|---|---|
| Stage 1 | Brief display flicker and indicator disturbance near the customer. | On stage-1 `CUSTOMER_INTRO` entry while the figure is beside the machine: at 0.4 seconds, a 0.15-second mild display dip and short nonsemantic indicator blink. Once per customer visit. |
| Stage 2 | Irregular selection-light pulses; subtle recurring three-light pattern. | At 0.5, 1.2 and 2.1 seconds after arrival, brief low-amplitude pulses using fixed authored intervals. A fixed group of three small status lamps glows together briefly at 2.5 seconds and recurs once at 5 seconds. Pattern never outlines the correct snack or maps lamps to a selection. |
| Stage 3 | Maintenance indicator flickers before interaction; nearby lights dim near the customer. | On `CUSTOMER_INTRO` entry start a 0.15-second indicator flutter before purchase becomes active; soften nearby machine lights over the first 0.4 seconds while the customer stands close. Do not delay an early Continue to force noticing. |
| Final reflection | Three-light motif during the reflection turn and light flicker. | Replay the same lamp grouping briefly at the start of the reflection turn and again during the disappearance flicker. Never add identifying text, a response, or an implied deliberate signal. |

The scene cue clock carries from `CUSTOMER_INTRO` into `MACHINE_2D` without replaying arrival effects; it stops and pending purchase cues are discarded on entry to the 3D transition. An early purchase can skip later cues, intentionally. Retry/reinitialize directly into rescue never replays arrival clues. New stage initializes only its own cues; returning to title/new game clears all cue state. Failure resets for remaining attempts do not affect already-finished customer clues. Small local brightness changes must preserve text and control contrast and avoid rapid full-screen strobing. Clue onset is deterministic for the same state/input timeline, independent of render frame count; intensity and durations remain tuning values, not artistic mandates.

### Outro subphases — proposed implementation of confirmed sequence

Use `CUSTOMER_EXIT → REFLECTION_LINGER → REFLECTION_TURN → FLICKER_VANISH → RETURN_TITLE` within `REFLECTION_OUTRO`. Suggested durations are 0.8, 1.0, 0.7 and 0.2 seconds respectively; tune them for legibility. The reflection remains after the body has left, turns without the absent customer, and disappears only during the flicker. The turn cue and final flicker cue reuse the exact three-lamp motif from stage 2. Clear result text on entry, ignore menu/gameplay input throughout, and reset the outro cursor on every new run. Timer updates commit each beat once, handle elapsed-time crossings in order, and never make rendering advance the sequence. Preserve visible linger/turn beats after a long frame by capping visual update delta; do not let a stall skip the entire ending. No puzzle, reveal caption, or extra ending follows.

## 9. Code plan grouped by six efforts

Use the repository's existing structure and course build setup. If there is no established structure, prefer one readable C++ source file for the first version, with clear section headings and functions, rather than adding an engine or dependency. Follow the user's Module 1 toolchain guidance: MSYS2 UCRT64 GCC and FreeGLUT, using the existing course workspace/build configuration if available. Do not install a second MinGW distribution.

### M0 — Preliminary environment verification (prerequisite)

Before M1, inspect the actual VS Code workspace/build task, MSYS2 UCRT64 GCC, FreeGLUT headers/libraries and configured OpenGL/GLU linkage. Compile and run the smallest windowed OpenGL/FreeGLUT program with that existing course toolchain. Record compiler/version, exact command or build task and observed result; diagnose the actual setup if it fails. Do not install another MinGW distribution or claim success without a visible run. M0 is a prerequisite aligned with Effort 1, not a seventh development effort or replacement roadmap.


### Effort 1 — Low: window, state, coordinates, and rendering shell

**Goal:** build a program that opens, draws placeholder versions of the two views, renders text, and redraws on state changes.

1. Define `GameState`/`ClawState` enums and the smallest understandable set of ordinary structs for game, claw, fixed stage data, animation and menu values. Assign one owner to each value; do not duplicate state as unrelated flags. Define named logical/render and gameplay dimensions separately.
2. After M0 succeeds, initialize GLUT once, set display mode with depth buffer, proposed window size/title, create the window, register callbacks and one update chain, initialize data, then enter `glutMainLoop`.
3. Establish bottom-left logical 2D coordinates, top-left GLUT mouse conversion, 16:9 letterboxed content viewport, and resize-safe shared layout rectangles. Resize changes presentation mapping, not world/gameplay dimensions.
4. Create a `display()` callback that clears color and depth buffers, branches on `GameState`, calls the matching scene renderer, draws the HUD, and swaps buffers if using double buffering.
5. Add small primitive/text helpers and explicit 2D, 3D, framebuffer, HUD and overlay pass setup/restore for projection/model-view, viewport, depth, lighting, blending, texturing and vertex-array state.
6. Establish the shared warm/cool palette, a 16:9 content viewport, and a machine-dominant nighttime composition with customer space and a bottom dialogue band. Render directly to the window initially; framebuffer pixelation is deferred to Effort 6. Create a 2D placeholder machine: cabinet, display glass, shelves, snack silhouettes, coin slot, buttons, and chute. Use at least three primitive types in visible objects (for example quads, triangles, and lines/polygons). Make at least five objects visually distinguishable. Add visible shelf rails or cabinet accents from a static client vertex array using `glEnableClientState(GL_VERTEX_ARRAY)`, `glVertexPointer()`, and `glDrawArrays()`; verify the legacy compatibility path in the existing course setup. The indexed snack mesh with `glDrawElements()` remains Effort 5.

**Course connection:** Module 01 Subtopic 2 (pp. 14, 24–31) for GLUT initialization/display; Module 02 (pp. 3–5, 20–23, 28–38) for vertices, primitives and color; Module 03 Subtopic 1 (pp. 4–10) for raster-positioned text. The visible `glDrawArrays()` compatibility check also verifies Module 04 Subtopic 1 (pp. 7–13); full indexed rendering remains Effort 5.

### Effort 2 — Low/medium: 2D purchase loop and GLUT inputs

**Goal:** the player can start a stage, understand the request, insert a coin, select the requested snack, and trigger the dimensional surprise.

1. Register normal/special key down/up, mouse, reshape and one timer callback. Callbacks validate and dispatch to action helpers, then request redraw; only action/update helpers mutate game data.
2. Convert top-left GLUT mouse coordinates through the actual content viewport into bottom-left logical coordinates. Share the exact control rectangles between drawing and hit testing; reject letterbox/outside clicks.
3. Enforce state/overlay ownership before dispatch. Menu navigation changes menu data only; gameplay mutations call named helpers; pause remains rescue-only and retry remains game-over-only except confirmed pause-menu Reinitialize.
4. Implement mouse actions: insert coin; click the requested snack button; collect the delivered snack and acknowledge its reaction under Section 7. Clicking a wrong selection should give a short visual/text response and must not advance the stage or consume a rescue attempt.
5. After a valid purchase, animate the snack briefly, set the jammed target, and enter a short transition to 3D. The sudden shift should be the gag: use the Section 8 jam, warning, camera shift, and rescue reveal sequence, with adjustable timing. Keep it achievable with GLUT timer callbacks rather than blocking waits.
6. Draw a simple geometric customer beside the machine and one or two dialogue lines near the bottom during intro/result states. Reuse geometric buildings, windows, pavement, and streetlamps for the cozy street; keep decorations behind active controls. Keep text within screen bounds; choose a bitmap font already supported by FreeGLUT.

**Course connection:** Module 03 Subtopic 2: display/keyboard callbacks (pp. 21–24), redisplay (p. 18), mouse callback (pp. 25–26), and timer callback concept (p. 30). Key-up, special-key, reshape, and the selected single bounded update loop are project/FreeGLUT needs; do not attribute them to those specific basic examples.

### Effort 3 — Medium: playable 3D claw and attempt rules

**Goal:** create a clear, controllable rescue loop with forgiving interaction and predictable resets. Run simulation on the single clamped elapsed-time/fixed-step update path defined in Section 6; rendering does not advance it.

1. Define a compact machine-interior coordinate system: X is left/right across the machine; Z is front/back depth; Y is vertical. Clamp claw X/Z to the compartment. Keep the descent and lift vertical so the first version avoids complex physics.
2. Switch to a perspective projection with `gluPerspective` and a fixed, moderately elevated oblique camera using `gluLookAt` (if GLU is configured in the course build). Enable and clear the depth buffer. Draw the compartment back, side walls, oversized shelves, packages, illuminated chute, and claw as chunky colored cuboids/triangles/quads. Use cool blue-green surroundings with isolated warm highlights and simple face shading; imply enormous industrial scale around the bounded play area. Keep the full playable route visible from the fixed camera.
3. Render the claw as a cable plus a small three-prong or two-prong shape. Show a floor target marker/projected reticle beneath the claw to help the player judge depth. Distinguish the requested snack with a subtle outline or highlight when it is within grab range.
4. Use held-key flags in keyboard-down/keyboard-up callbacks for smooth but precise movement. A timer callback updates X/Z by a fixed small amount and clamps to legal bounds; release immediately stops movement. Support WASD and/or arrow keys. Do not add inertia.
5. Use Space as a context-sensitive action: if the claw is empty, lower it and attempt a grab; if it holds a snack, release it. A grab succeeds when the target center is within a small horizontal/depth tolerance and the snack is reachable. On success, attach the snack to the claw and lift it. A carried snack follows the claw exactly; no random slipping.
6. When released, test whether the snack's X/Z location overlaps the chute bounds. If yes, transition to success. If no, decrement attempts and reset claw/snack safely to their start positions. A missed grab may also decrement an attempt as the proposed rule. Display remaining attempts clearly; never add a countdown.
7. Use a configurable starting attempt count. Three is the suggested prototype default, not a user-confirmed number. When attempts reach zero, enter GAME_OVER and offer retry. Retry restores that stage's attempt count, claw state, target position, and moving obstacle phase.
8. Add collision/bounds checks against fixed packages and shelves so the claw cannot move through solid objects. Prefer a small number of axis-aligned obstacle rectangles/boxes. Make sure every stage has at least one reachable route to the target and chute.

**Important correctness rule:** prevent invalid states such as carrying two snacks, grabbing a non-target package, dropping a carried snack without an attempt outcome, moving the claw outside the cabinet, or starting a new stage with stale attempts/held objects.

### Effort 4 — Medium: stages, obstacle animation, and narrative clues

**Goal:** reuse the rescue system for three distinct stages and build the customer mystery gradually.

1. Store the three stages in a fixed array. Include target snack type and position, fixed obstacles, optional moving obstacle bounds/speed, intro/reaction text, and one small clue flag. Use a loop to draw repeated shelf dividers and packages.
2. Stage 1 has an open path and functions as a tutorial. Stage 2 places the chocolate bar among nearby packages but preserves a clear approach. Stage 3 places the juice carton behind a slow moving restocking arm; use a timer to update its position between safe limits. No countdown timer is part of gameplay.
3. Reuse geometric industrial supports, machinery, and street decorations across all three stages without obscuring routes, controls, or clues. Keep repeated snack/package geometry in arrays. Give packages distinguishable colors and simple features (chips bag seam, bar wrapper, carton cap) using primitives.
4. Use Section 5 editable natural dialogue and Section 8 deterministic machine cues: stage-1 display/indicator disturbance, stage-2 irregular pulses/three-lamp motif, stage-3 pre-interaction indicator and proximity dimming. Keep them cumulative but deniable and independent of purchase correctness. Do not call customers aliens or imply machine awareness in any runtime text.
5. On successful chute delivery, record delivery once, show ITEM RECOVERED on returning to the customer, and use the Section 7 collection/reaction acknowledgments before advancing exactly once. Do not consume more attempts for a successful delivery. After stage 3, run the outro instead of loading a fourth stage.
6. Implement the final purely visual ending in 2D: customer exits; their reflection remains in the machine glass, turns toward the viewer, then disappears during a brief light flicker. Return to a normal “PLEASE INSERT COIN” screen with no explanatory text.

### Effort 5 — Medium/high: vertex arrays and course evidence

**Goal:** complete and document Module 04 Subtopic 1 vertex arrays and Subtopic 2 indexed rendering, building on the visible `glDrawArrays()` compatibility check from Effort 1.

1. Reuse and refine the visible Effort 1 shelf-rail/cabinet-accent vertex array; store additional static geometry only where it supports normal gameplay.
2. Use the legacy vertex-array API described in the original Module 4 plan: `glEnableClientState(GL_VERTEX_ARRAY)`, `glVertexPointer`, optional `glColorPointer`, `glDrawArrays`, then disable client states. Use arrays for a meaningful repeated set of objects, not just one invisible demonstration.
3. Use separate arrays/draw calls for primitive types that need different topology (e.g. triangles for snack faces and lines for shelf edges). Keep pointer lifetime valid through the draw call; static arrays are sufficient.
4. Module 04 Subtopic 1 demonstrates the legacy client-array API (pp. 7–13). Check configured FreeGLUT/OpenGL headers and flags. Do not switch to shaders/VBO/VAO APIs unless the actual starter project or instructor requires them. Module 04 mentions buffer objects conceptually (pp. 2–4), but that does not make them a project requirement. Avoid mixing immediate-mode and client-array state carelessly; set colors and enabled arrays explicitly around each draw helper.
5. If the 3D array path is difficult under the installed GL setup, first prove client arrays in the 2D repeated geometry, then complete the proposed indexed 3D snack mesh. Framebuffer pixelation is independent of both course rendering demonstrations and must not block them.

#### Meaningful indexed geometry alongside non-indexed arrays

Use `glDrawArrays(GL_LINES, ...)` for visible repeated shelf rails or cabinet accents from a static coordinate array. Use `glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices)` for a reusable snack-box mesh visibly drawn for multiple packages, including the target. Both paths must appear in normal gameplay, not in unreachable demo code.

For a simple uniformly colored cuboid without per-face normals, an appropriate mesh uses eight corner positions and 36 indices for 12 triangles. Each face consists of two triangles; indices reuse corner positions. Translate/scale the same mesh for packages rather than copying every vertex per object. If per-face colors or flat normals require separate vertex records, use 24 face-local records and explain why a position may occur in multiple records; do not claim eight vertices suffice for incompatible per-face attributes.

Enable `GL_VERTEX_ARRAY`, set `glVertexPointer` with the matching element layout, and ensure the optional color pointer has matching records. Pass the number of index entries (36 in the eight-corner example), not the number of triangles, to `glDrawElements`. Match the C++ index element type to `GL_UNSIGNED_SHORT`; every index must be less than the vertex count. Keep vertex, color, and index storage alive through the draw call. In this legacy client-memory path, ensure no buffer binding changes pointer arguments into unexpected offsets. Disable the client arrays afterward and explicitly restore color/depth/projection state as needed for HUD/text and immediate-mode helpers. Check triangle winding if face culling is enabled.

Document five answers using the actual implemented mesh: what data is stored; why an array is appropriate; what each vertex represents; which primitive is rendered; and why reuse avoids repeated specification. Also explain what an index refers to, how `glDrawElements` differs from `glDrawArrays`, which visible objects use each call, and any duplicated records needed for colors/normals. Include source function names and an annotated mesh or screenshot once available, never invented line numbers or screenshots.

**Requirement distinction:** R08 records the general assignment minimum as supplied: an appropriate vertex-array or indexed-vertex approach; R09 records its explanation requirement. Module 04 Subtopic 2 explicitly teaches indexed rendering with `glDrawElements()` (pp. 14–16, worked example pp. 20–22). This project retains both visible `glDrawArrays()` and `glDrawElements()` as its implementation recommendation, with the complete indexed snack mesh at Effort 5. Verify any rubric-specific wording from the actual project assignment, not by inferring it from the teaching slides.

### Effort 6 — Medium/high: visual polish, transitions, and recovery

**Goal:** make the switch funny and the whole game understandable without adding systems. Keep one GLUT update chain, separate gameplay and presentation clocks, and preserve frozen paused visuals.

1. Complete the quiet street title with INSERT COIN and purchase-only HOW TO PLAY. Add contextual rescue help only after the reveal; never place a full control list on the initial title.
2. Complete the separate commercial purchase and industrial rescue HUDs, ITEM RECOVERED result, and RECOVERY UNSUCCESSFUL failure presentation from Section 8. Show attempts and claw instructions only in rescue; keep controls readable.
3. Refine the existing jam → warning → camera shift → rescue reveal sequence from Section 8, using brief adjustable timings. Preserve the return transition and reflection ending. No cutscene system is needed.
4. Complete the rescue-only pause overlay and its Resume, Reinitialize, Controls, and Return to Street actions under Section 7. Freeze the darkened scene, confirm destructive resets, and resume exact progress. Implement the functional pause path during Effort 3; this phase refines presentation.
5. Add retry and stage-reset logic. On retry, clear carried-item state and held-key flags to avoid the claw moving unexpectedly.
6. Add keyboard handling for both lower/uppercase letters. Avoid GLUT blocking calls such as `Sleep`; use timer callbacks so the UI remains responsive.

**Later visual enhancement within Effort 6:** Once core gameplay works, implement the optional framebuffer path from Section 8: 320 × 180 world rendering, 640 × 360 alternative, nearest-neighbor presentation, and a separate legible text overlay as needed. Validate support and fallback behavior before relying on it. Refine geometric light pools, face shading, and optional fog only while the target, claw marker, obstacles, and chute remain visible. Update resize/mouse mapping and the PDF/README to describe the actual supported rendering path. Uniform pixelation may remain documented as deferred without invalidating the first playable milestone; do not call it delivered until observed.

### Integration notes for Efforts 2–6

The original phase descriptions above retain their intended scope. Apply Sections 6–7 for exact prototype behavior: `STAGE_RESULT` owns post-delivery collection; `MACHINE_2D` owns purchasing. Pause is rescue-only by default; direct R/Retry is game-over-only, while explicitly confirmed pause-menu Reinitialize intentionally uses the same stage-reset helper. Failures charge exactly once. Return to Street ends the run at TITLE, not at a paid purchase scene. Effort 5 includes both draw calls. Validate these changes at each effort's gate in Section 12 rather than leaving correctness until polish.

## 10. Suggested source organization

Prefer one organized C++ source file initially. The following are responsibilities and optional small structs/enums, not mandatory classes, modules, or new files. Keep the existing course structure if present; avoid an entity-component system, inheritance hierarchy, scene graph, event bus, engine, service layer, or unnecessary dependencies.

| Responsibility | Suggested data / functions | Boundary |
|---|---|---|
| Authoritative data | `GameData`, `ClawData`, fixed `StageData[3]`, `AnimationData`, `MenuData`; `GameState`/`ClawState` enums | Small structs own related values; avoid duplicate flags. OpenGL handles stay outside gameplay data. |
| Gameplay logic and collision detection | `initializeNewGame`, `initializeStage`, `returnToTitle`, `retryStage`, `transitionTo`, `updateClaw`, `updateObstacle`, `resolveFailure`, grab/chute tests, bounds/collision helpers | Authoritative mutations occur here; renderers read only. Collision uses world coordinates independent of pixelation. |
| 2D environment and character rendering | `drawStreet2D`, `drawBuilding`, `drawWindows`, `drawPavement`, `drawStreetlamp`, `drawMachine2D`, `drawCustomer`, `drawReflection` (read-only) | Draw from current state and reusable geometry; reserve customer/dialogue space and machine hit regions. |
| 3D environment and claw rendering | `setupRescueCamera`, `drawMachineInterior3D`, `drawIndustrialDetails`, `drawClaw`, `drawSnack`, `drawClawMarker`, `drawChute` | Fixed oblique camera, low-poly arrays/indices, face shading, visible navigation; all read-only. |
| Shared retro presentation and transitions | Palette/constants; content viewport, primitive/text helpers, `drawHud`, `drawDialogue`, `updateTransition`, `drawTransition`; later `initializePixelTarget`, `presentPixelTarget`, cleanup | Own visual parameters and optional framebuffer resources. Transition updates use existing game-state gates; presentation does not own duplicate gameplay state. |
| GLUT callbacks and timing | `display`, `reshape`, key/mouse adapters, one `timerUpdate`, `main` | Validate and dispatch input; read-only drawing; one elapsed-time update chain with fixed gameplay steps and separate presentation clocks. |

Keep includes/constants and data declarations at the top, then coordinate/hit-test helpers, gameplay helpers, draw helpers, callbacks, and `main`. Exact function names are suggestions. Keep related functions adjacent and only extract another file when the actual source becomes difficult to navigate.

`display` reads authoritative state, draws the scene directly or into the optional low-resolution target, presents it, draws that state’s HUD/dialogue/overlay, then swaps buffers; it never mutates gameplay. `reshape` updates viewport and mappings only. One timer reads elapsed time and dispatches fixed gameplay updates and relevant presentation updates. Mouse callbacks validate viewport then call action helpers. No callback duplicates transition logic.

Use a few named presentation constants for palette, world render dimensions, camera values, and transition durations; no generic configuration framework is needed. Share the same 2D layout rectangles between drawing and hit tests. Preserve both `glDrawArrays` and `glDrawElements` inside the scene renderers regardless of whether framebuffer presentation is enabled.

### UI rendering and update functions — proposed small helpers

These are coherent responsibilities, not a requirement for new files or classes. Use the same source organization and existing primitive/text helpers.

| Suggested function / data | Responsibility and state gate |
|---|---|
| `drawTitleUI`, `drawPurchaseHUD` | Title actions and purchase-only help entry; commercial request/credit/selection feedback in intro/purchase. Shared layout rectangles drive hit tests. |
| `drawRescueHUD`, `drawControlsHelp` | Industrial target/attempt/status and valid action hints; help content selected from owner/context. Never expose rescue instructions from TITLE. |
| `drawPauseMenu`, `drawConfirmation` | Darken frozen rescue, draw four options, focus outline and consequence/Cancel controls; read overlay state without mutating it. |
| `drawStageResultUI`, `drawFailureUI`, `drawDialogue` | ITEM RECOVERED plus collection/reaction phase, RECOVERY UNSUCCESSFUL plus Retry, editable customer lines. No such overlay in outro. |
| `handleMenuInput`, `activateMenuAction`, `setOverlay` | Consume keyboard/mouse once; validate owner/state; change selection, help/confirm overlay, resume or call existing reset helpers. Clear held actions at boundaries. |
| `updateMachineClues`, `drawMachineIndicators` | Scene clock and fired flags control local brightness only; render reads values. No identity/profile data or gameplay writeback. |
| `updateOutro`, `drawReflection` | Ordered outro subphase cursor, elapsed time, reflection pose and motif; transition to title once, never from a draw helper. |

`display` draws the active world, presents the pixel target if used, draws only that state's HUD/dialogue, then the active overlay. Paused rescue uses the unchanged world pose and a final darkening layer beneath the menu. `timerUpdate` skips rescue simulation while any pause-owned overlay is open; it still requests redraw for menu interaction. Customer clues update only in intro/purchase; outro has its own clock. Store only overlay mode/owner, menu selection, result subphase, cue elapsed/flags and outro subphase/elapsed in addition to existing state. Use existing stage reset/initialization helpers rather than duplicating resets in menus.

Use a single set of control labels and hit rectangles for drawing/help/input. Letterbox mapping, resize, focus outlines, bitmap text contrast, GL-state restoration, and edge-triggering apply to every menu as well as the world. Successful delivery cannot execute twice, and a draw call cannot fire a clue, collect an item, deduct an attempt, or advance the outro.

## 11. Course requirement map and traceability

| Requirement | Where the plan demonstrates it |
|---|---|
| C++ and OpenGL/GLUT | Main application, drawing helpers, registered callbacks. Use course FreeGLUT/UCRT64 setup. |
| Functions | Scene drawing, primitive helpers, callbacks, stage/reset/attempt logic. |
| Variables and conditions | Game state, attempts, target positions, claw state, hit tests, collisions, stage transitions. |
| Loops | Draw repeated shelves/packages; iterate through stage/package arrays. |
| Geometric primitives and coordinates | Machine, customers, snacks, claw, obstacles, chute, reflection made from vertices and primitive shapes. |
| Colors | Separate machine parts, packages, target highlight, state feedback. |
| GLUT callbacks | Display, reshape, keyboard, special-key, key-up, mouse, timer. |
| Text | Title, instructions, customer lines, requested snack, attempts, pause/result. Use GLUT bitmap text and raster positioning. |
| Arrays | Fixed stage definitions and repeated package data; vertex/color arrays for geometry. |
| Vertex arrays | Module 04 Subtopic 1 pp. 7–13: M1 compatibility demonstration and M5 reusable `glDrawArrays` geometry; Subtopic 2 pp. 14–16, 20–22: M5 indexed snack mesh via `glDrawElements`. |
| 2D and 3D | Orthographic 2D purchase/customer view; perspective 3D machine-interior rescue. |
| At least five objects / three primitive types | Clearly distinguish machine, customers, snacks, shelves, coin, buttons, claw, obstacles, chute; use at least three of points/lines/triangles/quads/polygons. |
| At least three application states | Title, customer/purchase, rescue, pause, result, game over, outro. |
| Extra feature | Dimensional surprise plus gradual visual mystery; moving restocking arm is an additional stage mechanic. |

### Requirement-to-evidence traceability

All entries are **planned evidence, currently unverified**. Replace planned function labels with actual source references after implementation.

| Requirement | Implementation / effort | Acceptance evidence / proposed tests |
|---|---|---|
| R01 | Module 01 Subtopic 2 (pp. 15, 19–21, 24–28); prescribed UCRT64/FreeGLUT build, M0 then M1 | Actual environment/build/run record and README; T01. |
| R02 | Module 01 pipeline/application (pp. 8–14); Module 02 vertices/primitives/color (pp. 3–5, 18–38); Module 03 callbacks/text (pp. 4–10, 18–26); Efforts 1–5 | Actual source examples and visible objects/HUD; T01–T03, T17, T20. |
| R03–R04 | Module 03 Subtopic 2 display/keyboard/mouse callbacks and redisplay (pp. 18, 21–26); Efforts 2–3, 6 | Control table and observed mouse/keyboard actions; T03–T16, T21. |
| R05 | Module 02 primitive/attribute examples (pp. 3–5, 18–38); machine, customer, snacks, shelves, claw, chute | Identify at least five readable objects in actual screenshots; T01. |
| R06 | Module 02 `GL_LINES` (p. 8), triangles (pp. 20–21), `GL_QUADS` (pp. 28–29); lines for rails/cable, quads for panels, triangles for low-poly snack faces | Source calls and screenshots of three visible primitive types; T01, T20. |
| R07 | Explicit state flow and pause overlay; Efforts 1–4, 6 | State diagram and observed transitions; T03–T19. |
| R08-R09 | Module 04 Subtopic 1 `glVertexPointer`/`glDrawArrays` (pp. 7-13), Subtopic 2 `glDrawElements` (pp. 14-16, 20-22); early compatibility check M1, complete implementation M5 | T01 verifies early visible draw; T20 checks both paths, data and five required array explanations plus actual index explanation. |
| R10 | Deterministic claw movement/collision and animation, moving arm; Efforts 3–4 | Demonstrate at least one working extra feature; T06–T13, T18. Narrative alone is not the technical evidence. |
| R11 | Section 13 test matrix | The official project minimum is 10 executed/documented cases; the 51-case matrix is broader recommended development coverage. Record observed results/evidence. |
| R12 | Seven-item submission package; Section 14 | Final artifact checklist and working links; documentation review, no present completion claim. |

The 2D/3D switch and mystery are preserved game design choices. They must not be mislabeled as official course requirements. A feature appearing in this map does not establish that it has been implemented.

### Visual-design evidence — confirmed design, not extra official requirements

| Design commitment | Effort / planned evidence |
|---|---|
| Cozy pixel-style nighttime street and machine composition | Efforts 1–2, 4; T25 and real title/purchase/result screenshots. |
| Surreal chunky industrial rescue with fixed camera/marker | Efforts 3–4; T26 and gameplay footage showing legal routes. |
| Shared palette, geometric lighting, safe optional fog | Efforts 1, 3, 6; T29 comparing exterior/interior and overlay visibility. |
| Consistent pixelation and readable text | Later Effort 6 enhancement; T27–T28 with actual render dimensions and fallback/deferred status documented. |
| Dramatic transition and subtle visual ending | Efforts 2, 4, 6; T30 and captured transition/outro, without explanatory runtime text. |

## 12. Acceptance criteria per effort

Each gate is **not yet verified**. Complete the gate for the current effort before expanding scope. These are implementation acceptance criteria, not additional official rubric items.

| Effort | Observable acceptance criteria | Planned checks |
|---|---|---|
| 1 — Rendering shell | M0 compiler/toolchain/window test has an actual recorded result; window can open/close in the existing course toolchain; title, text, and placeholder 2D/3D views display; at least five distinguishable objects and three visible primitive types; a visible legacy client-array demonstration renders through `glVertexPointer()` and `glDrawArrays()`; resize preserves readable projections and safe aspect calculations. Direct rendering establishes the nighttime machine composition, palette, and reserved customer/dialogue space; no framebuffer prerequisite or indexed mesh requirement. | T01–T02 plus M0 environment record; record actual build/run output. T25. |
| 2 — Purchase/input | Start/intro/purchase states are distinct; one coin enables the correct selection; wrong/no-coin selection cannot start rescue or consume attempts; resized hit boxes match the controls; correct selection triggers one responsive transition. Customer placement and bottom dialogue leave purchase controls unobstructed; the jam/warning/camera/reveal sequence runs once. | T02–T05, T16, T21. T25, T30. |
| 3 — Rescue loop | Movement uses clamped fixed-step updates, stops on release and stays within bounds; target-only forgiving grab works at the specified tolerance; carried snack follows exactly; valid chute release succeeds; each invalid grab/drop costs one attempt; zero attempts ends play; retry produces a clean same-stage state. Fixed moderately elevated oblique camera, visible marker, chunky industrial scale, and illuminated chute keep all legal routes readable. | T06–T12, T14, T22–T23. T26, T29. |
| 4 — Stages/story | All three stage definitions load in order with fresh state; fixed/moving obstacles leave a reachable target/chute route; collection advances only once; stage 3 enters the visual outro without out-of-range access; clues remain brief and deniable. Reused street/industrial detail stays moderate; decoration never changes collision or exposes the mystery; reflection remains legible. | T15, T17–T19. T25–T26, T30. |
| 5 — Arrays/evidence | Normal play visibly uses both non-indexed shelf geometry and indexed reusable snack geometry; index/type/count/lifetime checks are correct; HUD and other drawing remain intact; documentation answers all required array questions using actual data. Face shading and reusable decorations preserve correct array topology and both visible draw paths independently of pixelation. | T20; source inspection plus visual comparison. T20, T29. |
| 6 — Polish/recovery | All visual-critical targets and messages remain readable at both resolutions; HUD describes only valid state actions; pause freezes rescue and resumes without a jump/stuck key; transitions stay responsive; lower/uppercase controls agree; restart and resize preserve rules; the seven submission artifacts are prepared with truthful evidence. Check nearest-neighbor world presentation at both resolutions, text legibility, resize mapping, and optional fog visibility. If framebuffer enhancement is deferred, state that limit and verify direct-render fallback; do not claim uniform pixelation complete. | T02, T13–T16, T19, T21–T24; final deliverable review. T27–T30. |

Do not mark an effort complete solely because its code exists. Record the observed result and remaining defect before advancing. A failed check requires a fix or a plainly documented unresolved limit, not a fabricated pass.

### Integrated UI, writing, and mystery gates

| Existing milestone | Additional acceptance criteria — not yet verified | Planned cases |
|---|---|---|
| Efforts 1–2 | INSERT COIN and purchase-only help work with aligned mouse/keyboard input; commercial display has no rescue spoilers; transition text remains restrained. | T31–T33, T44 |
| Effort 3 | Contextual rescue help appears only after reveal; valid action prompts match claw state; four-option pause, nested help, confirmed restart/end-run and clean resume work before polish. | T32, T34–T37 |
| Effort 4 | Editable natural dialogue stays deniable; deterministic cues match stage triggers without affecting selection; result acknowledgment and all outro beats execute once. | T38–T43, T45 |
| Effort 6 | All UI shares readable palette/type/layout, darkening preserves menus, resized hit regions match; no banner or explanation reaches outro/title; evidence reflects actual implementation. | T31–T51 alongside original T01–T30 |

These extend the six existing milestones rather than adding new features or implementation phases. The artistic beats are confirmed; exact prose, timings, bindings, and helper names remain proposed defaults.

## 13. Proposed test cases — all unverified

These are **proposed tests, not performed tests**. The official CS0045 minimum is at least 10 executed and documented test cases. T01–T51 are a broader recommended development verification checklist, not 51 mandatory submission tests; all 51 are retained for development and regression checks. A representative core set of about 15 cases is T01, T03, T04, T05, T06, T07, T08, T11, T12, T13, T14, T15, T16, T17, and T19. Together they cover launch/normal purchase, invalid selection, mouse and keyboard input, movement boundaries, state restrictions/transitions, successful delivery, failure, retry, pause/resume, and all three stages/outro. The remaining cases remain available for additional verification and regression testing. None has been executed. Use the prototype rules in Sections 6–7 as expected behavior; update expectations if an adjustable design choice changes before execution. Original test identifiers T01–T30 remain; targeted wording updates clarify the INSERT COIN action, the legacy-array check, and the game-over retry inputs. Collection/progression checks include the separate collection and reaction acknowledgments specified in Section 7. “Controls while paused” in T16 means only the permitted menu actions work, not that menus are disabled. T31–T45 make these refinements explicit.

| Test | Scenario | Action / input | Expected result | Actual result | Status |
|---|---|---|---|---|---|
| T01 | Launch and visible course objects | Build/run with the documented existing course setup; inspect title and both scene renderers. | Window/text render; identify at least five objects and three actual primitive types; visible legacy client-array geometry renders through glVertexPointer/glDrawArrays; indexed mesh is deferred to Effort 5. No success claimed until observed. | Not executed | UNVERIFIED |
| T02 | Resize and mouse mapping | Resize wide/tall, minimize/restore, then click visible coin and selection controls. | Projection/text remain usable; hit boxes align; zero-size events are safe; letterbox clicks are rejected if used. | Not executed | UNVERIFIED |
| T03 | Normal purchase | Start, acknowledge intro, insert coin, select requested snack. | One purchase enters one jam/3D transition and the current rescue stage. | Not executed | UNVERIFIED |
| T04 | Selection without coin | Click correct and wrong snack buttons before inserting a coin. | No transition or attempt loss; insert-coin feedback appears. | Not executed | UNVERIFIED |
| T05 | Wrong selection / duplicate coin | Insert coin twice, select wrong snack, then correct snack. | Only one credit exists; wrong choice preserves credit and attempts; correct choice proceeds once. | Not executed | UNVERIFIED |
| T06 | Movement and release | Hold/release each WASD and arrow binding; test opposite and diagonal input. | Correct X/Z mapping; immediate stop; opposite directions cancel; diagonal speed is capped. | Not executed | UNVERIFIED |
| T07 | Cabinet and solid bounds | Move against all walls and fixed packages, empty and carrying. | Claw/snack footprints remain within legal space; no tunneling or passage through solids. | Not executed | UNVERIFIED |
| T08 | Forgiving target-only grab | Grab target inside and exactly on each tolerance boundary; aim at a non-target. | Reachable target is acquired at/below tolerance; no second/non-target snack can attach. | Not executed | UNVERIFIED |
| T09 | Missed grab charged once | Grab just beyond tolerance; hold Space through the descent and several update ticks. | Exactly one attempt is spent; safe reset occurs; repeat input cannot start another cycle without release/new press. | Not executed | UNVERIFIED |
| T10 | Stable carrying | Carry the target over a legal route; pause briefly and continue. | Snack maintains its attachment offset and never randomly slips. | Not executed | UNVERIFIED |
| T11 | Chute success boundary | Release with snack center inside and exactly on chute acceptance edge in separate runs. | Each valid release succeeds once, spends no attempt, and transitions to result. | Not executed | UNVERIFIED |
| T12 | Invalid drop | Release just outside chute, then allow several frames to pass. | Exactly one attempt is spent; claw/target/obstacle reset; no stale carry or delayed success. | Not executed | UNVERIFIED |
| T13 | Pause during motion and action | Pause while moving, descending, ascending, and carrying; wait, then resume. | All rescue/arm animation freezes; same progress resumes without time jump; fresh key press is needed for motion. | Not executed | UNVERIFIED |
| T14 | Exhaustion and retry | Fail until zero, attempt movement/Space, then press R, Enter while Retry is focused, and click Retry in separate runs. | Zero enters game over; gameplay is blocked; each activation calls the same `retryStage()` helper, restores the current stage attempts/layout without advancing progression, and clears held/action state. | Not executed | UNVERIFIED |
| T15 | Collect only once | After delivery, click collection repeatedly and press Enter. | Acknowledgment advances exactly one stage; event cannot also acknowledge the next intro or purchase. | Not executed | UNVERIFIED |
| T16 | State-invalid controls | Try purchase clicks in rescue; motion in title/result; R in active rescue; controls while paused. | Only Section 7 permitted actions occur; attempts, coin, and stage remain correct. | Not executed | UNVERIFIED |
| T17 | Three-stage progression | Complete and collect stages 1–3; inspect stage initial state. | Correct customer/target/layout loads; attempts and attachment reset; no fourth-stage array access. | Not executed | UNVERIFIED |
| T18 | Moving arm and reachability | Traverse stage 3 with empty and loaded claw at different arm phases. | No solid overlap or permanent trap; a route to target/chute exists; waiting costs no attempts. | Not executed | UNVERIFIED |
| T19 | Narrative outro | Finish stage 3 and watch the full ending. | Customer exits; reflection stays, turns, vanishes with light flicker; ordinary title returns without explicit reveal. | Not executed | UNVERIFIED |
| T20 | Arrays and indexed mesh | Inspect actual vertex/index arrays and both draw calls; observe shelves/snacks and HUD. | Visible glDrawArrays and glDrawElements geometry; indices/count/types valid; no dangling data or leaked client states; documentation matches mesh. | Not executed | UNVERIFIED |
| T21 | Unknown/repeated input | Press unknown keys, hold action keys including R/Enter at game over, click outside controls, and type uppercase controls. | Unknown/outside input is harmless; one-shot actions including retry do not repeat while held; letter-case behavior matches. | Not executed | UNVERIFIED |
| T22 | Input cleanup / visibility | Hold movement, trigger allowed pause/reset or lose visibility, then release and return. | No stuck movement; callback support/limitations are recorded if focus cannot be observed reliably. | Not executed | UNVERIFIED |
| T23 | Configurable attempts | Use a positive non-default startingAttempts value in the prototype, then fail/retry and change stage. | HUD, exhaustion, retry, and new-stage initialization use the configured value rather than a hidden literal 3. | Not executed | UNVERIFIED |
| T24 | Fresh game and exit | Finish the outro, Start again, then close the window during a subsequent session. | Fresh stage 1 has no stale credit/carry/keys/obstacle state; window closes through the intended lifecycle. | Not executed | UNVERIFIED |
| T25 | 2D visual composition | Inspect title, intro, purchase, and result at default and resized windows. | Glowing machine dominates cozy nighttime street; geometric buildings/windows/pavement/lamps repeat coherently; customer stands beside machine; bottom dialogue does not cover controls. | Not executed | UNVERIFIED |
| T26 | 3D scale, camera, and marker | Move empty and carrying claw through legal extremes in all stages. | Chunky industrial surroundings imply enormous scale; fixed elevated oblique view exposes route, target, marker, arm, and illuminated chute; decoration does not contradict collision. | Not executed | UNVERIFIED |
| T27 | Pixelation and fallback | When available, render both scenes at 320 × 180 and 640 × 360 into 1280 × 720; exercise direct-render fallback with enhancement disabled/unavailable. | World uses crisp nearest-neighbor 4×/2× scaling without orientation errors; both array draw paths remain visible; fallback remains playable and is labeled as lacking uniform framebuffer pixelation. | Not executed | UNVERIFIED |
| T28 | Text, resize, and presentation mapping | Read dialogue/HUD in both world resolutions; resize to non-16:9 and small windows; click controls and bars. | Text stays legible; content scales consistently; mouse mapping follows actual world/overlay rectangles; bars reject clicks; resolution never changes gameplay coordinates. | Not executed | UNVERIFIED |
| T29 | Lighting and atmospheric visibility | Inspect target, marker, obstacles, and chute with chosen face shading and any optional fog; switch between 3D and 2D/HUD. | Warm 2D and cool 3D palette contrast is clear; warm highlights guide play; no effect hides decisions; fog/lighting/texturing/depth state does not leak into HUD or 2D. | Not executed | UNVERIFIED |
| T30 | Transition and reflection readability | Observe jam, warning, camera shift, rescue reveal, return, and stage-3 reflection at both proposed resolutions when available. | Sequence is short and responsive; rescue starts only once camera is stable; reflection lingers, turns, and vanishes visibly with flicker; no explicit narrative reveal is added. | Not executed | UNVERIFIED |
| T31 | Title and purchase-only help | Launch; open/close HOW TO PLAY; activate INSERT COIN by Enter and mouse in separate runs. | Quiet street/title and correct start label; help teaches purchase only; fresh unpaid stage 1 starts once without input leaking into intro. | Not executed | UNVERIFIED |
| T32 | Contextual rescue teaching | Read every pre-reveal surface, then enter rescue and change claw substates. | No early rescue spoiler; readable industrial controls appear after stable reveal; move/lower/carry/release/status hints match actual allowed actions. | Not executed | UNVERIFIED |
| T33 | Commercial purchase display | Try no-coin, wrong-item, duplicate coin and correct purchase in each stage. | Ordinary request/credit/feedback; no profile matching, attempts display, machine speech or automatic correct selection; original credit rules hold. | Not executed | UNVERIFIED |
| T34 | Pause menu navigation | Pause each claw substate; navigate four entries by keyboard/mouse after resizing. | Frozen darkened scene; all four functional labels readable; focus/hit areas agree; one activation per event; no underlying gameplay action. | Not executed | UNVERIFIED |
| T35 | Pause help and resume | Open Controls while paused; use Back/Esc; resume with P/Esc or Resume after a wait. | Correct rescue help; returns to prior menu focus; exact substate resumes without time jump or held movement; P cannot bypass child overlay. | Not executed | UNVERIFIED |
| T36 | Confirmed reinitialize | Open restart confirmation while carrying; cancel; reopen and confirm. | Cancel changes nothing; confirm restores same-stage rescue attempts/layout and clears attachment/keys once, preserving completed purchase; active-play R remains ignored. | Not executed | UNVERIFIED |
| T37 | Return to street and new run | Cancel then confirm Return to Street; select INSERT COIN again. | Consequence is clear; cancel preserves run; confirmation ends run at quiet TITLE; fresh stage 1 has no coin, cues, overlay, outro or carry leftovers. | Not executed | UNVERIFIED |
| T38 | Success and failure copy | Deliver snack; inspect return/result; collect and Continue with repeated inputs; exhaust attempts separately. | ITEM RECOVERED is understated; collection and reaction use separate fresh inputs; progression once; RECOVERY UNSUCCESSFUL offers working same-stage Retry. | Not executed | UNVERIFIED |
| T39 | Three-customer dialogue | Read all requests/reactions in order at supported window sizes; inspect script data. | Natural gradually odd editable lines, no explicit alien identity or machine awareness; no clipping or blocked controls; repetition remains optional writing, not a puzzle. | Not executed | UNVERIFIED |
| T40 | Stage-1 cue timing | Repeat same intro/purchase input timeline at different frame rates; retry rescue. | One brief display/indicator disturbance near customer at authored onset; no arrival replay on rescue retry; no game-state changes. | Not executed | UNVERIFIED |
| T41 | Stage-2 pulses and motif | Wait through authored pulses and two motif occurrences; repeat, then purchase early. | Irregular intervals repeat deterministically; same three-lamp group recurs; early purchase discards pending cues; no correct-item hint or automated selection. | Not executed | UNVERIFIED |
| T42 | Stage-3 pre-interaction cue | Enter intro; inspect maintenance indicator and lights; Continue immediately in another run. | Indicator starts before purchase input and lights dim near customer; no forced clue wait, selection effect or identity text; early input cannot make cues gameplay gates. | Not executed | UNVERIFIED |
| T43 | Outro motif and ambiguity | Complete final collection/reaction; observe exit, linger, turn and flicker including a slow-frame run. | Body leaves first; reflection lingers and turns; same three-light motif accompanies turn/flicker; reflection vanishes during flicker; no HUD/banner/explanation; ordinary PLEASE INSERT COIN returns once. | Not executed | UNVERIFIED |
| T44 | Menu gates and transition copy | Try P/Esc/R/Enter and clicks through every non-rescue state, help/confirm overlays and both transitions. | Only allowed state actions occur; game-over Enter activates Retry only when focused; generic Start and superseded warning are absent from rendered UI; brief commercial jam notice precedes reveal; overlay events never leak. | Not executed | UNVERIFIED |
| T45 | Cue independence and render purity | Compare normal completion with fast inputs that miss clues; inspect draw/update ownership; restart a completed run. | Identical purchase/rescue rules and reachable ending without noticing cues; no draw-side mutation, random schedule, customer profile matching or stale cue/outro state. | Not executed | UNVERIFIED |
| T46 | Single update chain and bounded delta | Instrument or inspect timer registration; simulate/minimize during movement then restore. | Exactly one update chain; elapsed delta is clamped and fixed-step catch-up bounded; no teleport, duplicate animation or attempt charge. | Not executed | UNVERIFIED |
| T47 | Paused and presentation clocks | Pause during carrying and wait; inspect ambient title separately; resume. | Rescue world, claw, carried snack and arm remain visually frozen; resume has no accumulated movement jump; allowed title ambience is independent. | Not executed | UNVERIFIED |
| T48 | Coordinate conversion and letterboxing | Resize wide/tall, click title/purchase/menu controls at content edges and inside bars. | Top-left mouse converts to bottom-left logical space accurately; active shared rectangles match drawings; bars/outside points are rejected. | Not executed | UNVERIFIED |
| T49 | Gameplay invariance across render sizes | Compare movement, grab boundary, collision and chute edge at 320x180 and 640x360 with same world input. | Gameplay distances and outcomes match; render resolution does not alter speed, tolerances, collision or chute acceptance. | Not executed | UNVERIFIED |
| T50 | OpenGL pass-state isolation | Switch 2D, 3D, HUD, pause and optional framebuffer repeatedly. | Projection/model-view, viewport, depth, blend, lighting, texture and client arrays are set/restored; no state leakage or broken text/world. | Not executed | UNVERIFIED |
| T51 | Visual feedback readability | Inspect target, marker, obstacles, chute, focus and messages at both resolutions and supported window sizes. | Critical objects differ by silhouette/outline/shape as well as color; visible chute guide matches collision; text/focus readable; clues remain subtle and reflection perceptible. | Not executed | UNVERIFIED |

During execution, record build/version, OS/toolchain, relevant stage/configuration, observed actual result, and evidence reference. Use PASS only after observing the expected result; use FAIL or BLOCKED when appropriate. A planned expected result must never be copied into the actual-result column as if observed. No screenshots, recordings, test results, or build logs are supplied by this planning revision.

For enhancement-dependent cases, record BLOCKED/deferred when that path is not implemented or supported; test the direct-render fallback separately. Optional fog may be recorded as not enabled with its subcheck not applicable. Neither is an automatic PASS. All rows remain UNVERIFIED in this document. T46–T51 add timing, pause, coordinates, resolution-invariance, OpenGL state and readability checks.

## 14. Seven deliverables and PDF documentation outline

### Seven official deliverables

All seven are **required by the supplied assignment**. Suggested contents below are implementation/documentation recommendations. This revised handoff is not a claim that the submission package exists.

| # | Deliverable | Suggested contents / completion evidence | Current status |
|---|---|---|---|
| 1 | C++ source code | Actual compilable source and necessary project files; small course-compatible organization; meaningful callbacks, rules, arrays, and both proposed draw paths. | Not verified / not produced in this revision |
| 2 | PDF documentation | Use the outline below; real screenshots, actual source references, and truthful test outcomes. | Not produced |
| 3 | Complete ChatGPT conversation/share link | A complete accessible record covering planning and implementation; check the submitted link's access. Preserve the planning reference, but do not assume this cached preview is a complete conversation artifact. | Not verified |
| 4 | Video demonstration | Show purchase, 2D/3D transition, rescue, failure/retry, pause, stage variation, and ending; show title/help, four pause actions, state-specific UI and subtle clue/outro timing, plus exterior/interior contrast and the actual pixelation path; demonstrate five meaningful actions and mouse/keyboard. | Not produced |
| 5 | Screenshots | Label meaningful states, objects, controls, primitive examples, and array-rendered geometry; include exterior/interior, marker/chute, dialogue, and actual resolution examples; capture from the real program. | Not produced |
| 6 | 10 or more test cases | Execute the project-specific matrix and record actual results/status; keep unexecuted rows marked honestly. This handoff supplies 51 proposed development cases, including original T01–T30. | Proposed only; all UNVERIFIED |
| 7 | README with compilation/run instructions | Exact validated course environment, project layout, build/run steps, controls, known limitations, required runtime files, chosen render resolution, and supported framebuffer/fallback behavior. Do not present guessed commands as validated. | Not produced |

### Recommended PDF documentation outline

| Part | What to include |
|---|---|
| A — Project overview and proposal | Title, problem/idea, purpose, target users, graphical objects, user interaction, expected output; preserve the original seven-part proposal. |
| B — Requirements and scope | Official CS0045 requirement IDs and traceability; confirmed game decisions versus working defaults; first-playable scope and exclusions. |
| C — Design and flow | State diagram, three-stage data, Section 8 visual specification, cozy street/industrial contrast, fixed camera/marker, shared palette, adjustable pixelation/lighting/timing choices, coordinate axes, control table, exact attempt/grab/drop/pause/reset rules, UI render/input ownership, result subphases, editable dialogue versus confirmed copy, deterministic clue timelines and outro motif. |
| D — Course Concepts Applied | Explain application initialization/rendering (Module 01, Subtopic 2, pp. 14–31); vertices/primitives/colors (Module 02, Subtopics 1–2, pp. 2–38); GLUT input/display/redisplay and raster-positioned bitmap text (Module 03, Subtopics 1–2, pp. 4–30); vertex arrays and indexed rendering (Module 04, Subtopics 1–2, pp. 2–22). Map actual functions and game features to module/page references; distinguish course instruction from assignment requirements and project recommendations. |
| E — Vertex and indexed rendering | Both rendering paths, actual vertex/color/index layout, topology and reuse diagram, the five R09 explanations (stored data, reason, vertex meaning, primitive, reuse advantage), and the actual `glDrawElements()` implementation. Cite Module 04 Subtopic 1 pp. 7–13 and Subtopic 2 pp. 14–16, 20–22. |
| F — Beyond-basics feature | Explain an implemented feature such as collision, deterministic claw animation, or moving arm; include observed evidence. |
| G — Verification | Environment, actual build/run procedure and outcome, executed test table, screenshots and defect notes; distinguish expected from observed. |
| H — User guide and submission references | Installation/run guidance matching README, controls, video/screenshots, complete conversation/share link, course references, known limitations. |

The design handoff may state the hidden alien premise so implementation remains coherent. Player-facing text, title/HUD, dialogue, and ending must not explain it. In the PDF, keep any explicit premise in a clearly labeled design/spoiler discussion if needed; do not add a runtime reveal to make documentation easier. Preserve the original proposal's narrative intent rather than changing the game to match a checklist.

## 15. Build, verification order, and continuation

1. **M0 — Environment verification:** inspect project/workspace, VS Code build task, MSYS2 UCRT64 GCC, FreeGLUT headers/libraries and OpenGL/GLU linkage. Compile and visibly run a minimal FreeGLUT window; record compiler/version, exact command/task and observed result. Do not install another MinGW distribution. If setup fails, record cause/fix or blocker before M1.
2. **M1 / Effort 1:** build and run direct window rendering with the cozy street/machine composition, shared palette, text, at least three primitive types, and the visible `glVertexPointer()`/`glDrawArrays()` legacy compatibility demonstration. Defer framebuffer setup and the indexed mesh.
3. **M2 / Effort 2:** confirm logical coordinate and top-left mouse mapping, letterbox rejection, shared hit rectangles and purchase callbacks.
4. **M3 / Effort 3:** add and verify the chunky industrial 3D interior, fixed elevated oblique camera, marker, illuminated chute, depth buffer, and claw bounds.
5. Verify one complete rescue: valid grab, carried snack, valid chute delivery, return to 2D.
6. Verify failed grab/drop consumes only the configured attempt through `resolveFailure`; safe reset, rescue-only pause, menu confirmations and game-over retry work.
7. **M4 / Effort 4:** add stages 2 and 3; check moving arm reachability, natural dialogue and deterministic ambiguous clues.
8. **M5 / Effort 5:** complete and verify reusable glDrawArrays shelf geometry and the glDrawElements indexed snack mesh from actual arrays; inspect index bounds and draw-state cleanup. Document both paths and cite Module 04 Subtopics 1-2.
9. Integrate the four-option pause menu, contextual help and reflection outro; run all stages and confirm clues remain involuntary-looking, subtle and independent of gameplay.
10. **M6 / Effort 6:** only after core gameplay works, finalize Section 8 pixelation/framebuffer enhancement and verify both proposed resolutions, nearest-neighbor scaling, legible overlays, and fallback. Check resize/mouse mapping, atmosphere visibility, all displayed instructions, no countdown, no out-of-bounds movement, and no input leaking between states. Document deferred enhancement work honestly.

### UI and narrative integration in the existing build order

At steps 2–3 implement the quiet title, purchase-only help, ordinary purchase display and shared input rectangles. At steps 4–6 introduce contextual rescue instructions and the functional four-action rescue pause menu with nested help/confirmations, same-stage restart and title reset. At steps 7–9 integrate editable dialogue, stage-scoped clue clocks, collection/reaction acknowledgment and the final motif/outro. At step 10 verify T31–T51 alongside T01–T30; refine presentation without changing controls or lore. These are planning instructions for later implementation: this revision produces only the updated Markdown handoff.

### Milestone evidence and known issues

M0 and M1–M6 are checkpoints aligned to the existing six efforts; M0 is a prerequisite only. At each checkpoint record what changed, whether compilation and visible execution were actually verified, relevant observed test outcomes, defects, and incomplete features. Keep a concise known-issues/deferred-features record in the project documentation as work begins. Maintain a runnable build whenever practical; fix fundamental gameplay before optional polish. A checkpoint is not complete because code exists. Never copy expected outcomes into actual results or mark tests passed without execution. This handoff itself contains no new build, run, screenshot or gameplay evidence.

### Toolchain and evidence guardrails

Inspect the actual repository, starter source, headers/libraries, and VS Code build task before editing or issuing installation commands. Continue using MSYS2 **UCRT64 GCC + FreeGLUT** as prescribed by the existing course guidance. Do not mix UCRT64 libraries with a separate MinGW compiler distribution. Determine the actual GLU linkage from the project before using `gluPerspective`/`gluLookAt`; verify the compatibility-style API required by the modules.

The handoff does not supply a validated compilation command because the actual project configuration has not been inspected here. Write the exact successful command/task into README only after it has been run; if it fails, record location, cause, and fix or unresolved blocker. Do not describe a build as successful based on a proposed command or static review.

Retain the original incremental order above, applying the corresponding Section 12 gate at each milestone. For final verification, execute Section 13 and reconcile the seven deliverables. Screenshots, the video, and the PDF must describe the same implemented version.

### Next conversation instruction

Start by checking the user's existing project and course build setup, then turn this plan into a small implementation sequence. Preserve the stated confirmed decisions and label the attempt count and stage details as adjustable defaults. Build a runnable milestone before adding all polish. Explain each completed milestone in plain language and identify any required user choice only when it cannot reasonably be resolved from this handoff.

Start with M0 environment verification, then M1 / Effort 1 and the actual project setup. Use requirement IDs to track coverage, Sections 6–7 for ownership/timing/input/coordinate contracts, Section 8 for passes/readability, Section 12 for gates, and Section 13 for tests. Resolve course-documentation questions against the real assignment rather than promoting a recommendation into an official mandate. Keep every proposed test UNVERIFIED until executed and report build status only from actual results. Follow the single-update, state-owner and read-only-renderer boundaries; update the known-issues/deferred-features record at each checkpoint. Do not begin game implementation from this document-editing task alone.

Apply Section 8 UI, dialogue, ambiguity, machine-disturbance and outro rules from the relevant first milestone onward, as well as initial composition/camera work, while deferring framebuffer pixelation until core gameplay works. Preserve the confirmed look; tune render dimensions, lighting implementation, and animation durations based on observed readability.
