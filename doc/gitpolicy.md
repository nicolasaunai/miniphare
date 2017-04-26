# Repository Policy

This document explains how to use contribute to the code repository.

## Official and developer Repositories


### The official repository
The project PHARE has an official repository [here](https://hephaistos.lpp.polytechnique.fr/rhodecode/GIT_REPOSITORIES/LPP/phare/miniphare/miniphare). Each developer has its own clone of the official repository. Contributing to PHARE consists in sending a pull request or a patch on the official repo. The official repository has two branches:

- the **master branch**, where each commit is a working version of the code PHARE. A new commit on this branch results:
  - from a bug fix (minor revision)
  - from a new release (major revision)
- the **Develop** branch, where new features are progressively added in preparation for the next major release.





### The developer repository

You work on your repository as it pleases you. There nothing imposed here, you're at home. However, *in order to see your code merged to the official repository one day*, there are a list of advices that you may want to follow.

- Have a local develop branch that is regularly synced with the upstream develop branch. The more often you sync your code with upstream code, the easier your life will be at the time you do a pull request.

- Keep your local ```develop``` history clean by using ```git pull -- rebase usptream develop```. This should be equivalent to ```git pull upstream develop``` since we advise you don't merge any local code to your ```develop``` branch but only use it as a synced clone of the upstream one.

- In case you're adding documentation, fixing a bug, adding coding style etc. improvements here and there, we recommend you use ```git rebase``` to put your code in a condition of fast-forward merge to develop.

- In case you're developing a new feature, 3-way merge onto develop will be authorized in pull-requests. You may merge your synced develop branch into your feature branch.

- try to avoid merging a local branch onto another if it is not in a fast-forward condition. Local branches should remain as independent of one another as much as possible.

- Commit well, commit often. The more you commit the more you'll be able to rewind, amend, etc. your code.

- Take your time to write [good commit messages](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). This is important that the message explains what the code you add is about.

- Your **commit history** should appear as a **linear progression of the code** from its state before your feature, and after it. Think of your commits as a way for the reviewer to understand the various essential and progressive steps to implement your feature. Don't hesitate to use interactive rebasing to squash, delete, re-order commits to improve readability.



## Pull Requests and code review

Pull requests are the normal way to push new code into the official repository. Once a pull request is submitted, reviewers are chosen to review your code and decide whether it can be merged or not. This section explains the different points that will be looked at when reviewing your pull request, as well as how the review is done.


### Rules for the pull request

These are points concerning the whole pull request, independently from the code that is submitted:

- **The pull request should come with a descriptive message**. A short text explaining what the PR is about. Don't hesitate to explain the reviewer what you have in mind when submitting the code. Is it perfect in your view? If you see some imperfections, assumptions, caveats etc. that may help the reviewer understand, please mention it.
- **PR should no have more than 10-15 commits**. More commits make it difficult for the reviewer to do its job.
- **One aspect by PR** is a maximum. Do not submit a pull-request gathering different stuff together. Better is to split in several different pull-requests than a big inconsistent one. This will greatly ease the review process and speed up the integration of your code.


### Rules for the submitted code

The first and obvious rule is that the proposed code should:
- compile
- run OK
- pass the continuous integration (not implemented yet)

**Small improvements**: If your pull-request is a bug fix, proposes code refactoring, adds documentation or proposes improvements in coding style here and there, we don't want to keep track of which of your local branch the code was from. We prefer to add your commits with a linear history. Therefore in this case, *we impose your branch to be merged as a fast-forward.*

**Features:** If your pull-request implements a whole new feature, we want to keep track of the merge in our history. Therefore in this case we don't impose a fast-forward. In case of fast-forward, we will add a merge commit. In case of a 3-way merge, *we ask you to make sure there are no conflict between your code and the upstream develop branch* at the time of your PR.

**Style**: Your code style should respect the project's style, both in code formatting and design.  

**Commits:**: The commit history you propose should appear as a linear progression of the code, from the previous state of develop to the full integration of your code. In other words, the commit i should not come back on code added on commit i-N in your branch. Squash small commits together if they touch the same piece of code. Commit messages should appropriately describe the code added in the commit.

**Tests**: Preferably, your code should come with unit testing and good code coverage. If so, tests should all pass.

**Documentation**: Preferably, your code should come with documentation.

**files**: we do not accept binary files in the repository.


### Review process

The code review maximizes the chances we add code that works well, does not lead to future problems to maintain it, and that keep the commit history as clean as possible. Reviewers have three vote options:

- **approved**: your PR is approved, congratulations. Your code will be merged to the official repository.

- **in review**: this is the default vote. The code is not approved nor rejected. This phase lasts as long as all reviewers do not *all agree* to approve your PR - or as long as your PR is not rejected. During this phase, reviewers will ask you questions, comment/discuss your code, ask you TODOs. TODO comments should be addressed promptly and followed by an update of the PR.

- **rejected**: the PR is rejected, either the owner of the PR or team members having write access to the official repository can close the PR. There are limited reasons why your PR can be rejected:

  - code does not compile
  - at least one test fails
  - the PR re-writes the official commit history
  - The owner refuses to address TODO comments asked by a majority of reviewers
  - The owner fails to address TODO comments for more than a week (because there are lots of TODOs, or big ones).

In case of rejection, we encourage re-submission of an improved PR.



#### Review cheat sheet / pre-commit checklist

This section is what you should ask yourself as a developer who's about to submit a pull-request, or as a reviewer who's reviewing a pull-request. It gives various questions that we think are important to check for a PR to pass, and different answers that may be given to a pull-request.

* [Does the code compile and run OK?](#compileOK)
* [Do all tests (including already existing and those you may have added) pass?](#allTestOk)
* [Do you have more than 15 commits in your PR?](#oneAspect)
* [Do yo have more than 15 commits in your PR?](#commitNbr)
* [Do the commits incrementally add code? Does each commit represent an incremental step from the previous state of develop to the state of the branch?](#linearHistory)
* [Is the PR about a bug-fix, new documentation, code refactoring, improvements in coding style or slight improvements in an existing feature?](#smallUpdate)
* [Is the code style appropriate?](#codeStylePR)
* [Are there binary files in the PR?](#binary)
* [How to ask the PR owner something, and what to expect?](#askPRowner)
* [I've been asked TODO something, what should I do?](#todoOwner)



* *Does the code compile and run ok?*<a name="compileOK"></a>
  * **No**: Your PR will very probably be rejected. You need to make sure code compiles and runs ok before re-submitting your PR
  * **Yes**: Ok nice!

* *Do all tests (including already existing and those you may have added) pass?*<a name="allTestOk"></a>
  * **No**: You need all tests to PASS before your PR is accepted. Your PR will probably be rejected.
  * **Yes**: ok Nice!


* *Does my PR focus on only one aspect that is clearly explained in the PR message?*<a name="oneAspect"></a>
  * **No** : then I need to split it as long as I don't have one aspect per PR or explain why you think it still deserve a review
  * **Yes**: nice!


* *Do yo have more than 15 commits in your PR?*<a name="commitNbr"></a>
    * **Yes** Very probably your PR is too big. Either there is too much code or you have too many small commits. You will very probably have review comments suggesting either to squash commits to make fewer ones, or to split your PR in several ones, depending on the situation. Please explain in the PR message why you think this is still OK if you think so. In case you have good arguments, you may be asked to submit a patch so we can add your code without adding your commit history.
    * **No**: the number of commits is acceptable, you're good to go, we'll see what they are now.


* *Do the commits incrementally add code? Does each commit represent an incremental step from the previous state of develop to the state of the branch?*<a name="linearHistory"></a>
  * **No**: You have commits where you add stuff that is later changed in subsequent commits. This will pollute the history in the repository. You will very probably be asked to use interactive rebase to improve your commit history until they represent an incremental build of your feature, and update your PR. In case this is too complicated, we may ask you to give us a patch so we can add your code without adding your commit history.
  * **Yes**: that's the way to go, nice!


* *Is the PR about a bug-fix, new documentation, code refactoring, improvements in coding style or slight improvements in an existing feature?*<a name="smallUpdate"></a>
  * **yes**: is it in a condition to be fast-forward merged given the current state of upstream develop?
    * **No**: You need to rebase so that your code is in a fast-forward configuration
    * **yes**: Nice!
  * **No**: Your PR seems to be a whole new feature, is it in a fast-forward configuration with respect to the current state of upstream develop?
    * **Yes**: ah ok, then if merged, we will do a no-fast-forward merge to keep track of your feature branch in the history
    * **No**: No worries, 3-way merge are OK for new features. Does your code have any conflict with the existing tip of usptream develop?
      * **No**: Nice!
      * **Yes**: Your PR has conflict that only you can solve. It will probably be rejected and closed. Sync your local develop branch with the latest upstream develop and solve any conflict you have in your feature branch before re-submitting a new PR.


* *Is the code style appropriate?* <a name="codeStylePR"></a>
  * **Yes**: the submitted code has a style that is similar to PHARE standards. Both the format and the design patterns are OK.
  * **No**: The code formatting is too divergent from the main PHARE standard, which tries to follow Clean Code principles. Changes will be suggested by reviewers. Changes can also concern the design (wrong abstraction, wrong data structure, etc.). In case changes asked by the reviewer could take more than a week to be addressed, the PR may be rejected.


* *Are there binary files in the PR?*<a name ="binary"></a>
  * **Yes**: Sorry, we do not accept binary files in the main repository.
  * **No**: All good!


* *How to ask the PR owner something, and what to expect?* <a name="askPRowner"></a>
  * You should ask nicely :-) There are two main ways to ask a PR owner something: NOTE and TODO coments. A NOTE comment is when you just want to have a more precise explanation about something you don't fully understand. A TODO comment is when you ask the PR owner to fix something. Usually, a NOTE comment asking more information about the code comes from the code not being very clear, and should result in a TODO comment asking to clarify the code either by renaming, commenting, etc. TODO comments should be used when there is a localized and small thing to do, i.e. not something that would take another month of work. They should basically be doable for the PR owner in a matter of hours, up to a week, depending on its time. Once the TODO has been addressed, the PR owner is expected to "fix the TODO comment" and the reviewer to check that it's been addressed properly. Large change in the submitted code should not lead to TODO comments but should be discussed in terms of rejection. Once the TODO is fixed, the PR owner is expected to "updated the PR". Remember that a PR owner can always argue if he/she thinks the TODO is not relevant. This is democracy. TODO and NOTE comments can appear in the code diff sections to point at a specific point, or in the PR comment, as a global comment about the PR itself (e.g. squashing commits etc.).



* *I've been asked TODO something, what should I do?* <a name="todoOwner"></a>
    * You should do it :-) More seriously, if a reviewer has made a "TODO comment", you can always discuss if you don't understand or don't agree with the comment. If you agree it should be done, then do the modification and update the PR so that review can proceed up to acceptance :-) Remember that this is democracy, you can argue but you you have to convince and the majority applies. If your disagree to address a TODO against the majority of the reviewers, your PR may be rejected.
