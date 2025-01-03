# Release Guide

## Preparing for a Release
1. **Update Version Information**:
   - Update the `VERSION` file with the new version number.

2. **Run Tests**:
   ```sh
   make test
   ```

3. **Build the Release**:
   ```sh
   make ENV=prod
   ```

4. **Create a Distribution Package**:
   ```sh
   ./scripts/build/make-dist.sh
   ```

## Tagging the Release
1. **Create a Git Tag**:
   ```sh
   git tag -a vX.Y.Z -m "Release version X.Y.Z"
   ```

2. **Push the Tag**:
   ```sh
   git push origin vX.Y.Z
   ```

## Publishing the Release
1. **Create a Release on GitHub**:
   - Go to the GitHub repository.
   - Click on "Releases" and then "Draft a new release".
   - Select the tag and provide release notes.
   - Attach the distribution package.
   - Publish the release.

## Post-Release Tasks
1. **Update Documentation**:
   - Ensure all documentation is up-to-date with the new release.

2. **Notify Stakeholders**:
   - Inform stakeholders about the new release.
