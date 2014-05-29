{
  "targets": [
    {
      "target_name": "qrencode",
      "sources": [
        "src/qrencode.cc"
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "-lqrencode"
      ]
    }
  ]
}
