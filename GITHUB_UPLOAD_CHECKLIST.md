# GitHub Upload Checklist

Use this list before pushing this repository to GitHub.

---

## ✅ Already done (in this repo)

- [x] Folder structure: `hardware/`, `firmware/`, `libraries/`, `docs/`, `resources/`
- [x] Root `README.md`, `LICENSE` (MIT), `.gitignore`
- [x] Hardware README, firmware README, libraries README
- [x] **Credentials removed:** Blynk auth, WiFi SSID/password, and phone number in all firmware sketches replaced with placeholders (`YOUR_BLYNK_AUTH_TOKEN`, `YOUR_WIFI_SSID`, `YOUR_WIFI_PASSWORD`, `+ZZxxxxxxxxxx`)
- [x] `Config.example.h` for optional local config; `Config.h` is in `.gitignore`

---

## 🔲 Do before first push

### 1. Confirm no secrets remain
- Search the repo for any real tokens, passwords, or phone numbers:  
  `auth`, `pass`, `ssid`, `92312`, etc.
- If you use a local `Config.h` with real values, ensure it is **not** tracked (it’s in `.gitignore`).

### 2. Initialize Git and first commit
From the project root:

```bash
git init
git add .
git status
git commit -m "Initial commit: Auto-Solar-Cleaner PCB, firmware, libraries, docs"
```

### 3. Create the repo on GitHub
- On GitHub: **New repository** (e.g. `automatic-solar-panel-cleaner-pcb` or `auto-solar-cleaner`).
- Do **not** add a README, .gitignore, or license (you already have them locally).

### 4. Push to GitHub
```bash
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
git branch -M main
git push -u origin main
```

### 5. (Optional) Add repo description and topics
- In GitHub repo **Settings** or the **About** section: add a short description and topics, e.g. `arduino`, `altium`, `solar-panel`, `pcb`, `blynk`, `esp8266`, `automation`.

---

## 🔲 Optional but recommended

### Repository
- **Description:** e.g. “PCB and firmware for an automatic solar panel cleaning system (Arduino Mega, ESP-01, Blynk).”
- **License:** Already set to MIT in `LICENSE`; you can select “MIT” in GitHub’s license dropdown so it’s displayed.

### Local cleanup (optional)
- Remove duplicate files from the **root** (they exist in `docs/` or `resources/`):
  - `2021-06-28 (1).png`
  - `Blynk_Supported_Firmwares.JPG`
  - `PCB-Pinout.png`
  - `SCL-10A-Solar-charge-controller-user-manual.pdf`
- Keep `PCB-Pinout.psd` at root or move to `resources/` if you want it in the repo.

### After first push
- In **Blynk**: consider regenerating the project auth token (old one was in the code and is now public in git history if you had committed it before). Then use the new token only in local `Config.h` or in-sketch placeholders.
- Add a **Release** (e.g. “v1.0”) and attach `hardware/.../Final PCB (V1).zip` if you want a clear download for the manufactured PCB.

---

## 📁 What gets committed

- **Included:** Altium source (`.PrjPcb`, `.SchDoc`, `.PcbDoc`), Gerbers, BOM, DRC, firmware source, libraries, docs, images, PDFs.
- **Ignored (via .gitignore):** `Config.h`, `.vs/`, `__vm/`, `*.vcxproj`, `*.sln`, Altium `History/`, `__Previews/`, `Project Logs`, `*.LOG`.

---

## ⚠️ If you already pushed with real credentials

- Rotate **Blynk** auth token and **WiFi** password.
- Consider using `git filter-repo` or BFG to remove the old secrets from history, or create a **new repo** and push again after replacing secrets (already done in this folder).

---

You’re ready for GitHub once the “Do before first push” steps are done.
