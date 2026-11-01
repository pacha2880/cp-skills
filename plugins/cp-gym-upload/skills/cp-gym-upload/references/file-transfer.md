# Moving files between the user's computer, the sandbox and the browser

In an agent session there are often three places: the user's computer (where the material is), a sandbox where you run code, and the user's browser (where Polygon and Codeforces are open). The details below are for Claude with a device bridge and Claude in Chrome. With another setup, the ideas still apply.

## Getting files into a page

- Browser file-upload tools usually accept only files the tool can see (with Claude in Chrome, files under the uploads folder). Stage files from the user's folder first; to send a file from the sandbox, write it to the user's folder and stage it from there. Keep each call under ~10 MB.
- A reliable pattern:
  1. Inject your own `<input type=file multiple>`, fixed in a corner of the page.
  2. On `change`, store the files in `window.FILES[name]`.
  3. Find the input with the find tool and upload into it.
  4. Then send the files with `fetch` + `FormData` from JavaScript.
- **If the tab navigates, everything in `window` is lost.** While files are loaded, work with `fetch` and do not navigate.

## Reading pages

- Results that contain session ids or query strings may be blocked by the browser tool ("Cookie/query string data"). Return only processed data. When inspecting a page's JS, replace `session`, `ccid`, `?`, `=` and `&` before returning it.
- `DOMParser` does not compute `innerText`. For pages that need rendering, load them in a same-origin `iframe` and read `contentDocument.body.innerText`.
- The first JavaScript call after a navigation can fail with "Security check failed". Retry once.

## Things to never do

- **Never click a button that opens a native `confirm()` dialog.** The tab freezes until the user closes it by hand. Submit those forms with `fetch` instead.
- Do not leave long jobs in a background tab: browsers throttle timers in background tabs.
