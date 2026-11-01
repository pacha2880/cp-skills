# Codeforces Gym

There is no API for gyms. Everything is done from a logged-in Codeforces tab. Creating gyms needs **coach mode** on the user's account.

## Create

1. Go to `/gyms/new`. Fields: englishName, duration, visibility (**PRIVATE**), season, contestType, country, city, difficulty (hidden), websiteUrl, englishDescription, englishLogo.
2. Fill them with JavaScript, upload the logo with the file-upload tool, and click "Create training".

The logo should be about 250×250. If the user's logo is not square (e.g. 1×2), paste it centered on a transparent square canvas and resize.

## Problems

`problems/newFromContest` with the UID of the Polygon contest:

1. Check the letter in each `index` field.
2. Tick only the problems you want.
3. Click "Add problems".

## Updating imported problems

- "Update contest" on the gym dashboard (submit `#update-contest-form` with `fetch`) re-syncs every problem from Polygon without deleting them, **but it deletes the ghosts**. Upload them again afterwards.
- The other option is to delete a problem (`Delete problem`, `action=delete`) and import it again with the same letter.

Ask the user before doing either.

## Other quirks

- "External storage error" in the sidebar: open `/gym/edit/ID` and click "Save changes" without changing anything.
- The gym's Time Limit field needs at least 500 ms. For less, set it in Polygon and update the gym.

## Check every problem

The problem page must be `text/html` with a `.problem-statement` in English: title, TL/ML, images and samples.

If the page is `application/pdf`, the HTML statement failed in the package. Open `statements/.html/<lang>/problem.html` inside the Polygon package zip to see why. It is usually a markup issue from `statements.md`.

## Materials

From the ADM. page:

- Send `action=addResource` (resourceType statements, resourceLanguage ENGLISH/SPANISH/…, resourceName, resourceBytes) with `fetch(FormData)`.
- There is one Statements resource per language. To replace one, first delete the old one with `#delete-resource-form`; its `resourceIds` comes from the `.delete-resource-link` element's attribute.
