# Statements

## Polygon statement fields

Polygon stores each statement in fields:

- **legend**
- **input**
- **output**
- **notes**: sample explanations and clarifications
- **tutorial**: the editorial, optional

Write each field as Polygon LaTeX (a subset: Polygon converts it to HTML for Codeforces). Samples are not part of the text; they come from the tests marked as samples.

## Markup that works

- Math with `$...$` and `$$...$$`. Lists with `itemize` / `enumerate`. Bold with `\textbf`, monospace with `\texttt`.
- Figures: PNG or JPG, as
  ```latex
  \begin{center}\includegraphics[scale=0.5]{img.png}\end{center}
  ```
  with a scale of 0.35–0.6 depending on the image size. **Do not** use `figure` or `width=...\textwidth`: the HTML conversion breaks.
- Write `\item $x$`, never `\item$x$` (no space). The missing space breaks the HTML, and Codeforces then shows the problem as a PDF.

## Markdown → LaTeX

When the source is Markdown (Juki and most web judges):

- headings → the matching field;
- `**x**` → `\textbf{x}`;
- inline code → `\texttt{}`;
- lists → `itemize` / `enumerate`;
- tables → `tabular`;
- escape `_ % & # { }` outside math.

## Translations

- Translate into English when the original is not in English, and keep the original language as a second statement.
- Keep the names, variables and math identical across languages.
- Read the two side by side at the end.
- Translate sample explanations and the editorial too, when there is one.

## PDFs for the materials

Build one problem set PDF per language (and one editorial PDF per language when available). A minimal preamble that worked with Polygon's `olymp.sty`:

```latex
\documentclass[11pt]{article}
\usepackage[T1]{fontenc}
\usepackage[utf8]{inputenc}
\usepackage[provide=*,spanish]{babel}   % or english, etc.
\usepackage{olymp,graphicx}
\graphicspath{{../img/}}
```

Keep an official PDF, if one exists, as `*-official.pdf`.
