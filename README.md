# CS160
## Create branch
git checkout -b feature/new-feature
## git add files
git add .
## git commit changes
git commit -m "Add new feature"
## git push to remote repository
git push origin feature/new-feature
## git merge main
git checkout main
git pull origin main
git checkout feature/new-feature
git merge main
## Create pull request
## Go to your repository on GitHub, navigate to the "Pull requests" tab, and click "New pull request". Select your feature branch and create the pull request.

## Run project
cd build && cmake .. && make && cd .. && ./CSG
