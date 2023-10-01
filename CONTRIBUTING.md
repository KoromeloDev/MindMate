# How can I contribute?
You can:
* Report bugs
* Add improvements
* Fix bugs
* Add new translations or fix the current ones

# Reporting bugs
The best means of reporting bugs is by following these basic guidelines:

* First describe in the title of the issue tracker what's gone wrong.
* In the body, explain a basic synopsis of what exactly happens, explain how you got the bug one step at a time.
* Explain what you had expected to occur, and what really occurred.
* Optionally, if you want, if you're a programmer, you can try to issue a pull request yourself that fixes the issue.

# Adding improvements
The way to go here is to ask yourself if the improvement would be useful for more than just a singular person, if it's for a certain use case then sure!

* In any pull request, explain thoroughly what changes you made
* Explain why you think these changes could be useful
* If it fixes a bug, be sure to link to the issue itself.

# Adding a new translation
To create a new translation, you will need:
* Qt Linguist program
* Create a file in the `src/i18n` directory, in the format `language_territory.ts`. For example `ru_ru.ts` or `en_us.ts`.

If you don't know how to create a translation file correctly, you can copy an existing one and change the target language in Qt Linguist (Edit->Translation File Settings).
You can read more about Qt Linguist [here](https://doc.qt.io/qt-6/linguist-translators.html).
