# ImGui-Spectrum
The goal of this project is to provide the ease of use and quick C++ development of the ImGui library, while also embracing Adobe's [Spectrum](https://spectrum.adobe.com/) guidelines, thus giving a similar look and feel to Adobe's flagship products. 

This project aims at not changing the ImGui API at all, so you could swap between ImGui and ImGui-Spectrum without changing your code. 

ImGui-Spectrum is currently usable, but adaptation of Spectrum widgets is not fully complete. Everything is fully functional, but some widgets may not look like the Spectrum ones. It currently looks like this:

![example](https://user-images.githubusercontent.com/11432831/55342990-efe60900-545e-11e9-8dca-3d811dcd9eec.png)

## Using Imgui-Spectrum

### Moving from original ImGui to ImGui-Spectrum
This repository has exactly the same API as the original ImGui. You should be able to just swap to this without any issues. 

* If you just copied ImGui source files to your project, you can overwrite them with the files from this repository. Make sure to include `imgui_spectrum.h` and `imgui_spectrum.cpp`.
* If you are using ImGui as a submodule, you can change the remote url in your `.gitmodules` file and then run `git submodule sync`.

### Getting started with ImGui or moving from another GUI library
Please read ImGui's [README.md](./README.md).

### Using Spectrum in your code
Make sure you call `ImGui::Spectrum::StyleColorsSpectrum();` at the start of your application, after ImGui has been initialized. You can pick between Light and Dark theme, check `spectrum.h`.

Optionally, you can call `ImGui::Spectrum::LoadFont(size)` to load [Source Sans Pro](https://github.com/adobe-fonts/source-sans-pro). If you have access to it, you may want to swap it to AdobeClean-Regular. But as a reminder, AdobeClean is a [restricted font](https://www.adobe.com/products/type/font-licensing/restricted-fonts.html).

## Controls
* Button - matches Spectrum's Action button. Other buttons to be added (call to action, primary).
* Checkbox - ok
* Radio - ok
* Combo - ok, but not exact match. ImGui's Combo is a mix of Spectrum's Combo and Dropdown controls. 
* Input - more or less ok
* Drag and slider controls do not match Spectrum, but would be a pain to do since each is coded individually (int, float, etc). 
* Scrollbar - colors ok, Spectrum's bar is smaller when not active.
* Resize control - Spectrum doesn't have one (?), matches scrollbar colors.
* Collapsing Header - Spectrum doesn't have this, but it is a common feature of ImGui. Implemented, inspired by Spectrum's Call-To-Action button. 


## Planned features
Planned features include:
* Better controls, fixing the shortcomings above.
* More buttons types. Spectrum has many: Call-To-Action, Primary, Primary (quiet), Secondary, Secondary (quiet), Negative. 
* A Window (ImGui::Begin() .. End() pair) to create standard sidebar / header bar / panel.

In general, the plan is to *not* add the full range of Spectrum controls and their variations, but just enough to make a good looking C++ prototype application. 


## For ImGui-Spectrum developers
### Internal Changes
* Added spectrum.h with spectrum colors under `namespace Spectrum`.
* Added spectrum.cpp with `StyleColorsSpectrum()` similar to `ImGui::StyleColorsDark()`. 
* Added color parameter to RenderText, RenderTextWrapped, RenderTextClipped
* Added `ImGuiWindowTempData::IsComboPopup`, a `bool` field that is true in a Combo or ListBox popup window. Used in Selectable to change the hover and selected style.


The above functions are likely to cause merge issue in the future if ImGui's author decides to refactor them as planned.

### How to update ater ImGui releases:
0. If you don't yet have the original remote in your local copy (check with `git remote -v`), add it: 
```
git remote add ocornut https://github.com/ocornut/imgui.git
```

1. pull changes 
```
git pull master
git fetch --tags ocornut
```

2. Merge master branch release
```
git merge v1.xx
```

3. Merge eventual conflicts

4. Checkout and merge other branches. 

Note: you could checkout the existing branch in ImGui-Spectrum and merge the remote one, however the remote branch will not contain ImGui-Spectrum changes, and it will likely cause merge conflicts *again*, so it's easier to branch off ImGui-Spectrum master and merge the remote branch. 

For example, for the `docking` branch:

```
git checkout master

git checkout -b docking_v1.xx
git merge ocornut/docking
git push --set-upstream adobe docking_v1.xx
```

1. Commit and push your changes as usual (and/or make a pull request!). 
