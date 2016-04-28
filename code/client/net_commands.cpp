#include "lib/assert.h"
#include "lib/serialization.h"
#include "lib/seq_write.h"
#include "net_commands.h"

buffer SerializeShutdownNetCommand(linear_allocator *Allocator) {
  net_command_type Type = net_command_type_shutdown;
  seq_write S = CreateSeqWrite(Allocator);
  SeqWrite(&S, &Type, sizeof(Type));
  return S.Buffer;
}

net_command_type UnserializeNetCommandType(buffer Buffer) {
  Assert(Buffer.Length >= sizeof(net_command_type));
  return *(net_command_type*)Buffer.Addr;
}

buffer SerializeSendNetCommand(buffer Message, linear_allocator *Allocator) {
  net_command_type Type = net_command_type_send;
  seq_write S = CreateSeqWrite(Allocator);
  SeqWrite(&S, &Type, sizeof(Type));
  SeqWriteBuffer(&S, Message);
  return S.Buffer;
}

send_net_command UnserializeSendNetCommand(buffer Input) {
  serializer S = CreateSerializer(Input);
  net_command_type Type = *(net_command_type*)SerializerRead(&S, sizeof(net_command_type));
  Assert(Type == net_command_type_send);

  memsize MessageLength = GetRemainingSize(&S);
  send_net_command Command;
  Command.Message.Addr = SerializerRead(&S, MessageLength);
  Command.Message.Length = MessageLength;

  return Command;
}
