<span style="font-family: Helvetica, Montserrat, Arial, sans-serif; color: light grey">
<div style="text-align: center;">

# E27 Master Repository
</div>

<div style="text-align: center;">

## Navigation
</div>
<div style="text-align: center; color: grey; font-size:16;">

*| [General](General.md) |*
</div>
<div style="text-align: center; color: grey; font-size:16;">

*| [Demos](Demos.md) | [Setup](Setup.md) | [Structure](Structure.md) | [Syntax](Syntax.md) |*
</div>

# E27 Master Repo
This repository contains all major embedded systems and shared software/firmware used on E27.

The purpose of this repository is to provide a common location for access, development, testing, and maintenance for the software on E27's embedded systems. It is also intended to provide a common structure and unified expectations so that multiple individuals can contribute to different systems without losing organization or stepping on each other's toes.

This repository is meant to be accessible to both experienced contributors and new students who may not have an understanding of how collaborative development works.

# Repository Structure
At the highest level, this repository is organized into three types of software:
1. Systems
2. Services
3. Demos

```
/
├── systems /
│   ├── HV_BMS /
│   └── VCU /
├── services /
│   ├── util /
│   └── io /
└── demos /
    └── util_demos /
```

The exact folder structure may change slightly over time, but all new code should generally fit one of the categories.

## Systems
A **system** is a self-encapsulated and independently identifiable "system" on the vehicle.

Some examples include:
- High-Voltage Battery Monitoring System (HV-BMS)
- Vehicle Control Unit (VCU)
Each system should have its own directory and README.md.

The system README should contain information such as:
- What the system does
- Hardware the system runs on
- Required development tools and software
- How to build the system
- How to flash/deploy the system
- How to run tests
- How to perform bench testing
- Any system-specific contribution guidelines
A developer working on a system should be able to read that system's README and understand what they need to get started.

## Services 
**Services** contain generic and reusable software that can and should be used across all applicable systems.

Services should avoid containing system specific code at all code. If such code is necessary, it can be included in that system's folder using the correct nomenclature/file organization.

### *`util`*
Provides utilities for any system that might need it. This can include
- Interrupt handlers
- System clock functionality and tick increment
- PID controllers
- Small, reusable routines

### *`io`*
Provides common interfaces for hardware inputs and outputs.

These services are intended to provide a consistent interface for common devices and peripherals so that any system-level code can use it without modification.

Some examples include:
- GPIO
- ADC
- CAN
- I2C
- SPI
Whenever possible, system code should use these common interfaces rather than directly implementing hardware-specific behavior, as this makes any project more maintainable, usable, and likely to be used into the future.

# Workflow
## Branches
Each major system has a dedicated development branch, as well as services and demos.

For example:
```
main
  └── release-ready code
beta
  └── bench-testing environment
alpha
  └── latest development work
system_dev
  └── system development environment
```
The exact branch names should match the project conventions established in this repository.

### System Branches
System branches are where active development for that system takes place.

If you are working on the HV-BMS, all of your work occurs within the `hv_bms_dev` branch, which is then merged into `alpha` after a pull-request is opened and your code is reviewed by a moderator (a team member or approved associate). 

You should almost never commit code directly to any branch higher than the development branch for a system. Some exceptions to exist however, such as `.gitignore`,  `.gitattribute`, or `README.md` files, to name a few.

### *`alpha`*
This branch represents the most recent, approved development work done in the repository.

It very likely contains:
- Incomplete features
- Experimental code
- Untested or unbuilt code
- Changes that can break other systems or modules
Thus, `alpha` is inherently unstable and is unsafe to deploy to hardware, other than for testing purposes (even then, this is ideally what the `beta` branch is for). Compile and run this code at your own risk.

### *`beta`*
The `beta` branch contains code that has successfully been unit-tested and reviewed, but is not yet ready for deployment on the vehicle. This is usually due to a need to bench-test the system.

### *`main`*
The `main` branch represents released, stable software.

Code should only be merged into `main` when it meets the project or system's release expectations and should always have an accompanying release (again, exceptions do exist).

In general, this means the code has:
- Passed review by a team member or approved associate
- Successfully built and compiled for the device it will run on
- Passed any applicable unit tests outlined in the system's `README.md`
- Has had all known issues documented
- Meets all requirements for the release
Thus, the `main` branch should be regarded as the final source of truth for release software, i.e. what's on the car is what's in `main`.

## Development Guidelines
### Keep Changes Focused
A commit should accomplish one thing, whether that's the addition of a module within a system, modifications to a module within a system, or implementing a change to a module throughout a system (or systems in the event that a service undergoes a major change).

For example, these commits are much easier to review:
```
Add battery temperature fault detection
Fix ADC conversion for cell voltage measurements
Add timeout handling to CAN receive function
```
than a single change with:
```
Fix battery stuff, reorganize files, update VCU and config
```
or even:
```
sdfhjk;gas;fkghjas;dghjkf
```
Keeping commits focused makes code review faster and easier, allows for better project history understanding, and is more maintainable from a project management perspective, since smaller commits are easier to undo, amend. and track.

### Write Code for the Team
Contributing to this repository means that you are ultimately contributing code to the racing team. As such, everything you write will be read by someone else at some point.

Prefer code that is:
- Clear
- Consistent
- Commented or reasonably documented
- Easy to test
Avoid using patterns that only exist in your head. **When in doubt, it is better to do things in a way that is consistent and clear, than in a way that is convenient to you.**

If something is unusual, hardware-dependent, unsafe to change, or otherwise non-obvious, document why it works that way.

Comments should generally explain why something is being done rather than simply repeating what the code does.

### Respect Other People's Time
Before submitting code for review, make at least some effort to verify that what you have written is at the very least *working*.

This can include:
- Compiling or building for the desired system
- Writing and running unit tests
- Telling ChatGPT to write and run unit tests
- Checking that your changes didn't break other systems
If you can't do at least some of the above, please document that you didn't, as well as why.

Unfortunately, testing is very important, especially in safety-critical systems like those on a vehicle. Something properly compiling **does not mean it works**.

### Make Good Commit Messages
Commits should describe the change they introduce.

Good commit messages are concise and specific:
```
Add over-temperature fault detection
Fix CAN receive timeout handling
Update ADC scaling for cell voltage
```
Instead of messages like:
```
changes
fix
stuff
working
update
```
Whenever possible, make commits small enough that another developer can understand what changed without having to reconstruct the entire development process.

## Code Review
Any changes meant to move towards a release **need** to be reviewed by a Team Member or approved Associate before they are merged with `beta` or `main`.

When a pull request is submitted, the descriptions need to answer the following questions:
- What was changed
- Why it was changed
- How it was tested
- What hardware was tested, if applicable
- Any known limitations or remaining issues

Reviewers may ask questions or request changes. This is a normal part of development and should not be taken personally.

Everyone makes mistakes. The review process exists because finding mistakes before deployment is safer and better than finding mistakes while the vehicle is in use.

# General Expectations
When contributing to this repository:
- Ask questions when something is unclear
- Do not be afraid to say that you do not understand something
- Do not assume that unfamiliar code is wrong simply because it works differently than you expected
- Do not assume that existing code is correct simply because it is already in the repository
- Keep unrelated changes separate from the work you are currently doing
- Try to test changes before asking someone else to review them
- Document important decisions and unusual behavior
- Prefer simple and understandable solutions over unnecessarily complicated ones
- Treat code review as part of development, not as a final judgment of your work
- If you break something, communicate it promptly and help fix it

Hopefully, this process can help everyone involved learn and build something great for competition!