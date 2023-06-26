# Mayhem Windows Development Environment Example

This example covers some example workflows for compiling a Linux program on a Windows host and uploading it to Mayhem.

This repository contains a sample CMake project, once the set up steps are complete this can be opened in Visual Studio and built to test the workflow.

## Initial setup

For this example, we will assume Windows >= 10 and a recent Visual Studio installation.
This guide was created using Windows 10 Pro and Visual Studio 2022.

## Visual Studio Setup

Please ensure the following options are installed from the Visual Studio Installer:

- Desktop development with C++
- C++ for Linux Development
- C++ CMake tools for Linux

![Visual Studio Install Options](img/vsopts.png?raw=true "Visual Studio Install Options")

## Set up a build environment

### WSL

Visual Studio's Linux cross-compilation support recommends WSL to handle building of Linux programs.
To install, run `wsl --install` from Powershell, wait for the initial install to complete, reboot and open WSL again to complete the install.
Once installed, set up Visual Studio to use WSL for Linux builds by selecting it in the connection dropdown.

![Connection Dropdown](img/env.png?raw=true "Connection Dropdown")

Please reference https://learn.microsoft.com/en-us/windows/wsl/install for more information.

### Remote Linux / Virtual Machine

Alternatively, Visual Studio can be configured to use a remote Linux machine or a virtual machine set up with a SSH server as a build host.
The connection details can be added in the Connection Manager in Visual Studio.

![Connection Manager](img/connectionmanager.png?raw=true "Connection Manager")

Please reference https://learn.microsoft.com/en-us/cpp/linux/connect-to-your-remote-linux-computer?view=msvc-170 for more information.

## Set up a container engine

In order to build docker images / run the Mayhem CLI from Windows, a container engine capable of running Linux containers is required.
There are a few options here:

### Docker Desktop

Probably the best-supported option, but requires a license fee for larger organizations.
Please ensure compliance with the terms before proceeding.

In general, as WSL is already set up, setting up Docker is straightforward and requires just running through the installer.

Please reference https://docs.docker.com/desktop/install/windows-install/ for more information.

### Podman Desktop

Podman is an alternative to Docker, and should not require any additional fees.
Set up is similar to Docker desktop, and also relies on WSL by default.

Please reference https://podman-desktop.io/docs/installation/windows-install for more information.

### Install Docker inside existing WSL install

The default WSL distro is Ubuntu, and Docker can be installed on top of this like any other Ubuntu install.
This option will not integrate well with the rest of the Windows ecosystem though, so commands will need to be run directly in WSL.

Please reference https://docs.docker.com/engine/install/ubuntu/ for more information if you choose to go with this route.

Note that the rest of this guide will not apply to this setup, however the rest of our documentation covering standard Linux setups should be sufficient in getting you running.
Please consult the Mayhem documentation for more information.

## Set up a CMake project

Using Visual Studio, create or open an existing CMake project.
You can create a new CMake project using the following option:

![New CMake Project](img/cmake.png?raw=true "New CMake Project")

Assuming your Linux build environment is set up properly, please set the connection dropdown to reference your Linux environment and ensure the "Linux Debug" build configuration is selected.

![Connection Dropdown](img/env.png?raw=true "Connection Dropdown")

If you would like to build Docker images from the Windows host side, we can add `"copyBuildOutput": true` to the "linux-debug" build target in `CMakePresets.json`, e.g:

![Copy Build Output](img/copybuildoutput.png?raw=true "Copy Build Output")

This option will copy the built files (executable) from the build environment (Linux) back to the Windows host on each successful build.

## Building and pushing a Docker image

We can then set up a Dockerfile to build a Docker image, an example is included in the code of this repository.

If building the image on a Windows host using copied build output, we can directly reference this with an appropriate base image to simplify the build step.
When moving to a CI pipeline, most likely we will want to build the program inside of Docker as well.
Please consult the Mayhem documentation for more details on CI integration.

We can build the image with `docker build -t TAG .` from the root of the project.
Replace `TAG` with a docker image reference, referencing the docker registry of your Mayhem instance for example (usually the hostname of your Mayhem instance plus `:5000`, e.g. `mayhem.internal:5000/my-image`).

We now need to push the image to the external registry.
If using the Mayhem docker registry, first ensure you've logged in to the registry.
To do this, first create an API token from the Mayhem UI and then use `docker login REGISTRY_URL` (with the appropriate registry URL), and login with the user `mayhem` and your API token as your password.

You can now push the image with `docker push TAG`, where `TAG` is the same tag used when building the image in the previous step.

## Run the target on Mayhem

A Mayhemfile is required to instruct Mayhem on how to analyze the target, an exmaple is included in the code of this repository.

Assuming a container engine is installed on the Windows host, we can run the Mayhem CLI through Docker.
We will assume `docker` is being used, feel free to substitute in `podman` etc if appropriate.

To create a run, from the root of the project we can execute:

```sh
$Env:MAYHEM_URL="..."
$Env:MAYHEM_USER="..."
$Env:MAYHEM_TOKEN="..."
docker run --rm -v ${PWD}:/workdir -e MAYHEM_URL=${Env:MAYHEM_URL} -e MAYHEM_USER=${Env:MAYHEM_USER} -e MAYHEM_TOKEN=${Env:MAYHEM_TOKEN} forallsecure/mcode-cli run .
```

Substituting in appropriate values for `MAYHEM_URL`, `MAYHEM_USER`, and `MAYHEM_TOKEN` to be set in the environment in the first three commands.

All other uses of the CLI can be invoked in the same way, please reference the Mayhem documentation for more information.
