import json, re, sys
import xml.etree.ElementTree as elementTree

class TsFileTranslation:
    def __init__(self, xml_source_file: str, xml_target_file: str):
        self.xml_source_file = xml_source_file
        self.xml_target_file: str = xml_target_file

        self.source_keys: list = list()
        self.map_translation: dict = dict()
        self.content: str = str()

        with open(self.xml_source_file, "r", encoding="utf-8") as fread:
            self.content = fread.read()

            match_qt_locale = re.search(r'language="([^"]+)"', self.content)

            if match_qt_locale:
                qt_locale = match_qt_locale.group(1)
                translation_map_file = r"translationMap_" + qt_locale + ".json"

                with open(translation_map_file, "r", encoding="utf-8") as fr:
                    self.map_translation = json.load(fr)
            else:
                raise ValueError("Can not find QtLocale")

    @property
    def source_keys(self):
        return self._source_keys

    @source_keys.setter
    def source_keys(self, value):
        self._source_keys = value

    @property
    def map_translation(self):
        return self._map_translation

    @map_translation.setter
    def map_translation(self, value):
        self._map_translation = value

    @property
    def content(self):
        return self._content

    @content.setter
    def content(self, value):
        self._content = value

    def get_source_keys(self):
        self.source_keys = re.findall(r'<source>\s*(.*?)\s*</source>', self.content, re.DOTALL)
        self.source_keys = [item.strip() for item in self.source_keys]
        self.source_keys = list(set(self.source_keys))
        self.source_keys = sorted(self.source_keys)

    def print_source_keys_json(self):
        print("\n{")

        for i in range(len(self.source_keys) - 1):
            _key = self.source_keys[i]
            _key = _key.replace("\n", "\\n")
            print(f'\t"{_key}": "",')

        print(f'\t"{self.source_keys[-1]}": ""')
        print("}\n")

    def print_compare_source_keys(self):
        RED = "\033[31m"
        GREEN = "\033[32m"
        RESET = "\033[0m"

        translation_keys = set(self.map_translation)
        source_keys = set(self.source_keys)

        print("Compare keys:\n")
        print("{")

        # x in self.map_translation && x not in .ts
        for key in sorted(translation_keys - source_keys):
            value = self.map_translation[key]
            print(f'\t{RED}- "{key}": "{value}"{RESET}')

        # x in .ts && x not in self.map_translation
        for key in sorted(source_keys - translation_keys):
            print(f'\t{GREEN}+ "{key}": ""{RESET}')

        print("}\n")

    def translate_ts_file(self):
        tree = elementTree.parse(self.xml_source_file)
        root = tree.getroot()

        for message in root.findall(".//message"):
            source = message.find("source")
            translation = message.find("translation")

            if source is None or translation is None:
                continue

            source_text = source.text

            if source_text in self.map_translation:
                translation.text = self.map_translation[source_text]
                translation.attrib.pop("type", None)

        tree.write(self.xml_target_file, encoding="utf-8", xml_declaration=True)

        print("File translated")

        with open(self.xml_target_file, "r", encoding="utf-8") as fread:
            self.content = fread.read()

        self.content = re.sub(' type="unfinished"', '', str(self.content))

        with open(self.xml_target_file, "w", encoding="utf-8") as fwrite:
            fwrite.write(self.content)

        print("Done")


if __name__ == '__main__':
    print("Arguments:", sys.argv)

    # Source File Path
    xmlSourceFilePath = sys.argv[1]
    xmlTargetFilePath = r"output.ts"

    obj = TsFileTranslation(xmlSourceFilePath, xmlTargetFilePath)

    # First Execution: get source keys for json
    obj.get_source_keys()
    obj.print_source_keys_json()
    obj.print_compare_source_keys()

    # Check .ts file: <TS version="2.1" language="xx_XX">

    # Translate .ts
    obj.translate_ts_file()

    # Add Manually line 2: <!DOCTYPE TS>
