import os
import re

def findfiles(directory, folder):
    return [os.path.join(directory, folder, x) for x in os.listdir(os.path.join(directory, folder)) if os.path.isdir(os.path.join(directory, folder, x))]

def clean(fileName, prefix):
    reNum = re.compile(r'(\d.*):', re.IGNORECASE)
    reCamR = re.compile(r'(- cam_R_m2c:)', re.IGNORECASE)
    isSceneGt = fileName[-6:] == 'gt.yml' and prefix == 'scene_'    

    output = '%YAML 1.0\n---\n'
    with open(fileName, 'r') as f:
        for line in f:
            if isSceneGt:
                if re.match(reNum, line) is not None:
                    output += prefix + line
                elif re.match(reCamR, line) is not None:
                    output += '  -\n   ' + line[1:]
                else:
                    output += '  ' + line
            elif re.match(reNum, line) is not None:
                output += prefix + line
            elif re.match(reCamR, line) is not None:
                output += ' ' + line[1:]
            else:
                output += line

    with open(fileName, 'w') as text_file:
        text_file.write(output)

def main():
    folder = './'
    for sceneFolder in findfiles(folder, 'scenes/primesense/'):
        print('Cleaning: ' + sceneFolder)
        clean(os.path.join(sceneFolder, 'info.yml'), 'scene_')
        clean(os.path.join(sceneFolder, 'gt.yml'), 'scene_')
    
    for tplFolder in findfiles(folder, '400x400/primesense/'):
        print('Cleaning: ' + tplFolder)
        clean(os.path.join(tplFolder, 'info.yml'), 'tpl_')
        clean(os.path.join(tplFolder, 'gt.yml'), 'tpl_')

    folder = './'
    for sceneFolder in findfiles(folder, 'scenes/kinectv2/'):
        print('Cleaning: ' + sceneFolder)
        clean(os.path.join(sceneFolder, 'info.yml'), 'scene_')
        clean(os.path.join(sceneFolder, 'gt.yml'), 'scene_')
    
    for tplFolder in findfiles(folder, '400x400/kinectv2/'):
        print('Cleaning: ' + tplFolder)
        clean(os.path.join(tplFolder, 'info.yml'), 'tpl_')
        clean(os.path.join(tplFolder, 'gt.yml'), 'tpl_')

    print('FINISHED')

if __name__ == '__main__':
    main()